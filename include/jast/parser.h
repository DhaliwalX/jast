#ifndef PARSER_H_
#define PARSER_H_

#include "jast/context.h"
#include "jast/string-builder.h"
#include "jast/tokens.h"
#include "jast/token.h"

#include <vector>
namespace jast {
class ASTBuilder;
class Tokenizer;
class Expression;
class Declaration;
class ExpressionList;
class FunctionPrototype;

// A recursive descent parser plus operator precedance parser for JavaScript
class Parser {
public:
    Parser(ParserContext *ctx, ASTBuilder *builder, Tokenizer *lex)
        : ctx_{ ctx }, builder_{ builder }, lex_{ lex }
    { }

    ~Parser();

    Expression *ParsePrimary();

    Expression *ParseArrayLiteral();
    Expression *ParseObjectLiteral();

    Expression *ParseDotExpression();
    Expression *ParseIndexExpression();
    Expression *ParseMemberExpression();
    Expression *ParseNewExpression();
    Expression *ParseCallExpression();
    ExpressionList *ParseArgumentList();
    Expression *ParseLeftHandSideExpression();

    Expression *ParsePostfixExpression();
    Expression *ParseUnaryExpression();

    Expression *ParseBinaryExpression();
    Expression *ParseBinaryExpressionRhs(int precedance, Expression *lhs);
    Expression *ParseTernaryExpression();
    Expression *ParseAssignExpression();
    Expression *ParseCommaExpression();

    Expression *ParseStatement();
    Expression *ParseBlockStatement();
    Expression *ParseVariableStatement();
    Expression *ParseIfStatement();
    Expression *ParseElseBranch();
    Expression *ParseContinueStatement();
    Expression *ParseBreakStatement();
    Expression *ParseReturnStatement();
    // Expression *ParseWithStatement();
    // Expression *ParseLabelledStatement();
    Expression *ParseSwitchStatement();
    Expression *ParseThrowStatement();
    Expression *ParseTryCatchStatement();
    // Expression *ParseDebuggerStatement();

    Expression *ParseForStatement();
    Expression *ParseWhileStatement();
    Expression *ParseDoWhileStatement();

    Expression *ParseFunctionStatement();
    std::vector<std::string> ParseParameterList();
    FunctionPrototype *ParseFunctionPrototype();

    Expression *ParseCaseBlock();
    Expression *ParseDefaultClause();

    Expression *ParseExpression();
    Expression *ParseExpressionOptional();

    Declaration *ParseDeclaration();

    Expression *ParseProgram();

    Expression *ParseForInStatement(Expression *inexpr);

    Expression *ParseVariableOrExpressionOptional();

private:
    String GetStringLiteral();
    String GetIdentifierName();
    double ParseNumber(const Token &token);

    TokenType peek();
    void advance(bool not_regex = false);

    ASTBuilder* builder() { return builder_; }
    Tokenizer* lex() { return lex_; }
    ParserContext *context() { return ctx_; }
private:
    ParserContext *ctx_;
    ASTBuilder *builder_;
    Tokenizer *lex_;
};

extern Expression *ParseProgram(Parser *parser);

}

#endif
