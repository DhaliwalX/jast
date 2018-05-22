#ifndef CODEGEN_H_
#define CODEGEN_H_

#include "jast/astvisitor.h"

namespace jast {

class CodeGeneratorInternal;

class CodeGenerator : public ASTVisitor {
public:
    CodeGenerator();

    void Visit(IntegralLiteral *literal);

    void Visit(BlockStatement *statement);

    static void Initialize();
private:
    CodeGeneratorInternal *internal_;
};

}

#endif
