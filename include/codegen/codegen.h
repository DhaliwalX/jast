#ifndef CODEGEN_H_
#define CODEGEN_H_

#include "jast/astvisitor.h"

#include "llvm/Target/TargetMachine.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"

namespace jast {

class CodeGeneratorInternal;

class CodeGenerator : public ASTVisitor {
public:
    CodeGenerator();

    void Visit(IntegralLiteral *literal);

    void Visit(Identifier *identifier);

    void Visit(MemberExpression *expression);
    void Visit(ArgumentList *list);
    void Visit(AssignExpression *expression);

    void Visit(BlockStatement *statement);

    static void Initialize();

    std::unique_ptr<llvm::Module> ReleaseModule();

    llvm::TargetMachine &getTargetMachine();

    llvm::LLVMContext &getLLVMContext();
private:
    CodeGeneratorInternal *internal_;
};

}

#endif
