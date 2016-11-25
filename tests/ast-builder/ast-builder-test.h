#ifndef AST_BUILDER_TEST_H_
#define AST_BUILDER_TEST_H_

#include "common/test.h"
#include "parser/ast-builder.h"
#include "parser/context.h"

namespace test {

class AstBuilderTest : public Test {
public:
    AstBuilderTest();
    ~AstBuilderTest();

    bool run() override;

    void init(grok::parser::ParserContext *context) override;

    void destroy() override {}

private:
    grok::parser::ASTBuilder *builder_;
};

}

#endif // ast-builder-test.h