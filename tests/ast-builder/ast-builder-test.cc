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
    Expression *expr = builder_->build; \
    log() << "Test " #expr "\n";  \
    assert(expr->As##expr() != nullptr); \
    log() << "Test " #expr " [Passed]\n"; \
    delete expr; \
} while (false)

bool AstBuilderTest::run() {
    log() << "Running AstBuilderTest" << "\n";

    TEST_AST_BUILDER_FOR(NullLiteral, NewNullLiteral());
    TEST_AST_BUILDER_FOR(UndefinedLiteral, NewUndefinedLiteral());
    TEST_AST_BUILDER_FOR(ThisHolder, NewThisHolder());
    TEST_AST_BUILDER_FOR(IntegralLiteral, NewIntegralLiteral(10));
    TEST_AST_BUILDER_FOR(StringLiteral,
        NewStringLiteral(std::string("grok")));

    {
        ProxyArray arr;
        TEST_AST_BUILDER_FOR(ArrayLiteral,
            NewArrayLiteral(std::move(arr)));
    }

    {
        ProxyObject obj;
        TEST_AST_BUILDER_FOR(ObjectLiteral,
            NewObjectLiteral(std::move(obj)));
    }

    TEST_AST_BUILDER_FOR(Identifier, NewIdentifier("grok"));
    TEST_AST_BUILDER_FOR(BooleanLiteral, NewBooleanLiteral(true));
    TEST_AST_BUILDER_FOR(ArgumentList, NewArgumentList(nullptr));
    TEST_AST_BUILDER_FOR(CallExpression,
        NewCallExpression(MemberAccessKind::kCall, nullptr, nullptr)
    );

    TEST_AST_BUILDER_FOR(MemberExpression,
        NewMemberExpression(MemberAccessKind::kCall, nullptr, nullptr));

    TEST_AST_BUILDER_FOR(NewExpression, NewNewExpression(nullptr));

    TEST_AST_BUILDER_FOR(PrefixExpression,
        NewPrefixExpression(PrefixOperation::kIncrement, nullptr));

    TEST_AST_BUILDER_FOR(PostfixExpression,
        NewPostfixExpression(PostfixOperation::kIncrement, nullptr));

    TEST_AST_BUILDER_FOR(BinaryExpression,
        NewBinaryExpression(BinaryOperation::kAddition, nullptr, nullptr));

    TEST_AST_BUILDER_FOR(AssignExpression,
                         NewAssignExpression(nullptr, nullptr));
    TEST_AST_BUILDER_FOR(TernaryExpression,
                         NewTernaryExpression(nullptr, nullptr, nullptr));

    TEST_AST_BUILDER_FOR(CommaExpression,
                        NewCommaExpression(nullptr));

    TEST_AST_BUILDER_FOR(IfStatement,
        NewIfStatement(nullptr, nullptr));

    TEST_AST_BUILDER_FOR(IfElseStatement,
        NewIfElseStatement(nullptr, nullptr, nullptr));

    TEST_AST_BUILDER_FOR(ForStatement,
        NewForStatement(ForKind::kForOf, nullptr, nullptr, nullptr, nullptr));

    TEST_AST_BUILDER_FOR(WhileStatement,
        NewWhileStatement(nullptr, nullptr));
    TEST_AST_BUILDER_FOR(DoWhileStatement,
        NewDoWhileStatement(nullptr, nullptr));

    TEST_AST_BUILDER_FOR(BlockStatement,
        NewBlockStatement(nullptr));

    TEST_AST_BUILDER_FOR(FunctionPrototype,
        NewFunctionPrototype("grok", {"a", "b"}));

    TEST_AST_BUILDER_FOR(FunctionStatement,
        NewFunctionStatement(nullptr, nullptr));

    TEST_AST_BUILDER_FOR(LabelledStatement,
        NewLabelledStatement("label", nullptr));

    TEST_AST_BUILDER_FOR(BreakStatement,
        NewBreakStatement(nullptr));

    TEST_AST_BUILDER_FOR(ContinueStatement, NewContinueStatement(nullptr));
    TEST_AST_BUILDER_FOR(CaseClauseStatement,
                         NewCaseClauseStatement(nullptr, nullptr));
    TEST_AST_BUILDER_FOR(SwitchStatement,
                        NewSwitchStatement(nullptr, nullptr));

    TEST_AST_BUILDER_FOR(TryCatchStatement,
                        NewTryCatchStatement(nullptr, nullptr, nullptr, nullptr));
    TEST_AST_BUILDER_FOR(ThrowStatement,
                        NewThrowStatement(nullptr));
    TEST_AST_BUILDER_FOR(ReturnStatement,
                        NewReturnStatement(nullptr));
    log() << "Passed all ASTBuilder tests" << "\n";
    return true;
}

}
