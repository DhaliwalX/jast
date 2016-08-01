#ifndef PARSER_H_
#define PARSER_H_

#include "parser/context.h"
#include "parser/string-builder.h"
#include "parser/tokens.h"

#include <vector>
namespace grok {
namespace parser {
class LexerInfo;
class ASTBuilder;
class LexerInfo;
class Expression;
class Declaration;
class ExpressionList;
class FunctionPrototype;

// A recursive descent parser plus operator precedance parser for JavaScript
class Parser {
public:
    Parser(ParserContext *ctx, ASTBuilder *builder, LexerInfo *lex)
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
    Expression *ParseArguments();
    ExpressionList *ParseArgumentList();
    Expression *ParseArgument();
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
    Expression *ParseEmptyStatement();
    Expression *ParseExpressionStatement();
    Expression *ParseIfStatement();
    Expression *ParseElseBranch();
    Expression *ParseIterationStatement();
    Expression *ParseContinueStatement();
    Expression *ParseBreakStatement();
    Expression *ParseReturnStatement();
    Expression *ParseWithStatement();
    Expression *ParseLabelledStatement();
    Expression *ParseSwitchStatement();
    Expression *ParseThrowStatement();
    Expression *ParseTryStatement();
    Expression *ParseDebuggerStatement();

    Expression *ParseForStatement();
    Expression *ParseWhileStatement();
    Expression *ParseDoWhileStatement();

    Expression *ParseFunctionStatement();
    std::vector<std::string> ParseParameterList();
    FunctionPrototype *ParseFunctionPrototype();

    Expression *ParseCaseBlock();
    Expression *ParseCaseClause();
    Expression *ParseDefaultClause();

    Expression *ParseExpression();
    Expression *ParseExpressionOptional();

    Declaration *ParseDeclaration();

private:
    String GetStringLiteral();
    String GetIdentifierName();
    double ParseNumber();

    TokenType peek();
    void advance();

    ASTBuilder* builder() { return builder_; }
    LexerInfo* lex() { return lex_; }
    ParserContext *context() { return ctx_; }
private:
    ParserContext *ctx_;
    ASTBuilder *builder_;
    LexerInfo *lex_;
};

}
}

#endif
