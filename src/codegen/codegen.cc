#include "codegen/codegen.h"

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

#include <iostream>

namespace jast {

static llvm::LLVMContext context_;

class CodeGeneratorInternal {
public:
    CodeGeneratorInternal()
    : builder_(context_)
    {}

    llvm::IRBuilder<> &builder() {
        return builder_;
    }

    llvm::LLVMContext &context() {
        return context_;
    }
private:
    llvm::IRBuilder<> builder_;
};

CodeGenerator::CodeGenerator()
: internal_{ new CodeGeneratorInternal() }
{ }

void CodeGenerator::Visit(IntegralLiteral *literal) {
    std::cout << "Generating code for " << literal->value() << std::endl;

    llvm::Value *v = llvm::ConstantInt::get(internal_->context(), llvm::APInt(32, literal->value(), true));
    v->dump();
}

void CodeGenerator::Visit(BlockStatement *statement) {
    for (auto &expr : *statement->statements()) {
        expr->Accept(this);
    }
}

void CodeGenerator::Initialize() {
    using namespace llvm;
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
}

}

