#include "ast-builder/ast-builder-test.h"
#include "parser/expression.h"
#include <string>
#include <iostream>
#include <cassert>

std::ostream &log() {
    return std::cout;
}

namespace test {
using namespace grok::parser;

AstBuilderTest::AstBuilderTest() {

}

AstBuilderTest::~AstBuilderTest() {
    delete builder_;
}

void AstBuilderTest::init(ParserContext *context) {
    Test::init(context);

    builder_ = new ASTBuilder(context,
                              ASTFactory::GetFactoryInstance(),
                              new DummySourceLocator());
}

#define TEST_AST_BUILDER_FOR(expr, build) \
do { \
    Expression *expr = build; \
    assert(expr->As##expr() != nullptr); \
    delete expr; \
} while (false);

bool AstBuilderTest::run() {
    log() << "Running AstBuilderTest" << "\n";

    TEST_AST_BUILDER_FOR(NullLiteral, (builder_->NewNullLiteral()));
    TEST_AST_BUILDER_FOR(UndefinedLiteral, (builder_->NewUndefinedLiteral()));
    TEST_AST_BUILDER_FOR(ThisHolder, (builder_->NewThisHolder()));
    TEST_AST_BUILDER_FOR(IntegralLiteral, (builder_->NewIntegralLiteral(10)));
    TEST_AST_BUILDER_FOR(StringLiteral, (builder_->NewStringLiteral(std::string("grok"))));

    log() << "Passed all ASTBuilder tests" << "\n";
    return true;
}

}
