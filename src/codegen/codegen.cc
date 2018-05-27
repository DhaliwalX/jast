#include "codegen/codegen.h"

#include "codegen/SymbolTable.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include <iostream>

namespace jast {

static llvm::LLVMContext context_;

llvm::LLVMContext &getGlobalContext() {
    return context_;
}

class CodeGeneratorInternal {
    friend class LoadEnvironment;
    friend class CodeGenerator;
public:
    CodeGeneratorInternal()
    : builder_(context_),
      target_machine_(llvm::EngineBuilder().selectTarget()),
      data_layout_(target_machine_->createDataLayout()),
      module_(nullptr)
    {
        module_ = std::make_unique<llvm::Module>("anon_module", context());
        module_->setDataLayout(getTargetMachine().createDataLayout());

        AddInitFunction();
                // create a pass manager attached to it.
        fpm_ = llvm::make_unique<llvm::legacy::FunctionPassManager>(module_.get());

        // do simple peephole optimizations
        fpm_->doInitialization();
    }

    void AddInitFunction() {
        using namespace llvm;

        FunctionType *funcType = FunctionType::get(Type::getVoidTy(context()), false);
        Function *function = Function::Create(funcType, Function::ExternalLinkage,
                                "__init__", module_.get());

        BasicBlock *bb = BasicBlock::Create(context(), "entry", function);
        builder().SetInsertPoint(bb);
        builder().CreateRetVoid();
        builder().SetInsertPoint(&*(bb->getFirstInsertionPt()));

        // Add print function to the module
        Type *charType = (Type::getDoubleTy(getGlobalContext()));
        std::vector<Type *> Doubles(1, charType);
        // Doubles.push_back(Type::getInt32Ty(getGlobalContext()));
        FunctionType *FT =
          FunctionType::get(Type::getVoidTy(getGlobalContext()), Doubles, false);

        Function *F =
          Function::Create(FT, Function::ExternalLinkage, "print", module_.get());

        AddSymbol("print", F);
    }

    llvm::TargetMachine &getTargetMachine() {
        return *target_machine_;
    }

    const llvm::DataLayout &getDataLayout() const {
        return data_layout_;
    }

    llvm::IRBuilder<> &builder() {
        return builder_;
    }

    llvm::LLVMContext &context() {
        return context_;
    }

    llvm::Module *ReleaseModule() {
        return module_.release();
    }

    llvm::Value *GetSymbol(const std::string &name) {
        if (table_.count(name)) {
            return table_[name];
        } else {
            return nullptr;
        }
    }

    void AddSymbol(const std::string &name, llvm::Value *val) {
        table_[name] = val;
    }

    std::vector<llvm::Value*> &stack() {
        return stack_;
    }

protected:
    void SetLoad(bool val) {
        load_ = val;
    }
private:
    llvm::IRBuilder<> builder_;
    std::unique_ptr<llvm::TargetMachine> target_machine_;
    const llvm::DataLayout data_layout_;
    std::unique_ptr<llvm::Module> module_;
    std::map<std::string, llvm::Value*> table_;
    std::unique_ptr<llvm::legacy::FunctionPassManager> fpm_;

    std::vector<llvm::Value*> stack_;

    bool load_;
};

class LoadEnvironment {
public:
    LoadEnvironment(CodeGeneratorInternal *internal)
    : internal_{internal}
    {
        internal_->SetLoad(true);
    }

    ~LoadEnvironment() {
        internal_->SetLoad(false);
    }

private:
    CodeGeneratorInternal *internal_;
};

CodeGenerator::CodeGenerator()
: internal_{ new CodeGeneratorInternal() }
{ }

llvm::TargetMachine &CodeGenerator::getTargetMachine() {
    return internal_->getTargetMachine();
}

std::unique_ptr<llvm::Module> CodeGenerator::ReleaseModule() {
    return std::unique_ptr<llvm::Module>(internal_->ReleaseModule());
}

llvm::LLVMContext &CodeGenerator::getLLVMContext() {
    return internal_->context();
}

void CodeGenerator::Visit(IntegralLiteral *literal) {
    llvm::Value *v = llvm::ConstantFP::get(internal_->context(), llvm::APFloat(literal->value()));
    internal_->stack().push_back(v);
}

void CodeGenerator::Visit(BlockStatement *statement) {
    for (auto &expr : *statement->statements()) {
        expr->Accept(this);
    }
}

// XXX: FixMe: Identifier can also appear on RHS
void CodeGenerator::Visit(Identifier *identifier) {
    llvm::Value *val = internal_->GetSymbol(identifier->GetName());

    if (val == nullptr) {
        val = internal_->builder().CreateAlloca(
                        llvm::Type::getDoubleTy(internal_->context()),
                        nullptr,
                        identifier->GetName()
                        );
        internal_->AddSymbol(identifier->GetName(), val);
    }

    if (internal_->load_) {
        val = internal_->builder().CreateLoad(val);
    }
    internal_->stack().push_back(val);
}

void CodeGenerator::Visit(AssignExpression *expression) {
    expression->rhs()->Accept(this);
    expression->lhs()->Accept(this);

    auto lhs = internal_->stack().back();
    internal_->stack().pop_back();

    auto rhs = internal_->stack().back();
    internal_->stack().pop_back();

    internal_->builder().CreateStore(rhs, lhs);
}

void CodeGenerator::Visit(ArgumentList *list) {
    for (auto &expr : *list->args()) {
        expr->Accept(this);
        llvm::Value *val = internal_->stack().back();
        internal_->stack().pop_back();
        internal_->stack().push_back(val);
    }
}

void CodeGenerator::Visit(MemberExpression *expression) {
    {
        LoadEnvironment env(this->internal_);
        expression->member()->Accept(this);
    }
    expression->expr()->Accept(this);

    llvm::Value* expr = internal_->stack().back();
    internal_->stack().pop_back();

    llvm::Value* member = internal_->stack().back();
    internal_->stack().pop_back();

    if (expression->kind() == MemberAccessKind::kCall) {
        std::vector<llvm::Value*> args;
        args.push_back(member);

        if (auto *F = llvm::dyn_cast<llvm::Function>(expr)) {
            internal_->builder().CreateCall(F, args);
        } else {
            std::cout << "Not a function" << std::endl;

        }

    }
}

void CodeGenerator::Initialize() {
    using namespace llvm;
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
}

}

