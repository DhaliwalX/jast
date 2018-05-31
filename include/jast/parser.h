#ifndef PARSER_H_
#define PARSER_H_

#include "jast/context.h"
#include "jast/string-builder.h"
#include "jast/tokens.h"
#include "jast/token.h"
#include "jast/handle.h"
#include "jast/scope.h"

#include <vector>
namespace jast {
class ASTBuilder;
class Tokenizer;
class Expression;
class Declaration;
class ExpressionList;
class FunctionPrototype;


// ParserFlags := represents various parsing flags
class ParserFlags {
public:
    enum Flags {
        kNonRegex = 1 << 1,
        kAutomaticSemicolonInsertion = 1 << 2,
        kForInLoopParsing = 1 << 3,
    };

    bool IsForInLoopParsing() { return flags_ & Flags::kForInLoopParsing; }
    void SetForInLoopParsing(bool flag) {
        flag ? flags_ |= Flags::kForInLoopParsing : flags_ ^= Flags::kForInLoopParsing;
    }

private:
    // flags are packed inside 64 bit unsigned integer
    uint64_t flags_;
};

// A recursive descent parser plus operator precedance parser for JavaScript
class Parser {
public:
    friend class NewScope;
    friend class NonRegexEnvironment;
    friend class ForInLoopParsingEnvironment;
    
    Parser(ParserContext *ctx, ASTBuilder *builder, Tokenizer *lex, ScopeManager *manager);

    ~Parser();

    Handle<Expression> ParsePrimary();

    Handle<Expression> ParseObjectMethod(const std::string &name);
    Handle<Expression> ParseArrayLiteral();
    Handle<Expression> ParseObjectLiteral();

    Handle<Expression> ParseDotExpression();
    Handle<Expression> ParseIndexExpression();
    Handle<Expression> ParseMemberExpression();
    Handle<Expression> ParseNewExpression();
    Handle<Expression> ParseCallExpression();
    Handle<ExpressionList> ParseArgumentList();
    Handle<Expression> ParseLeftHandSideExpression();

    Handle<Expression> ParsePostfixExpression();
    Handle<Expression> ParseUnaryExpression();

    Handle<Expression> ParseBinaryExpression();
    Handle<Expression> ParseBinaryExpressionRhs(int precedance, Handle<Expression> lhs);
    Handle<Expression> ParseTernaryExpression();
    Handle<Expression> ParseAssignExpression();
    Handle<Expression> ParseCommaExpression();

    Handle<Expression> ParseStatement();
    Handle<Expression> ParseBlockStatement();
    Handle<Expression> ParseVariableStatement();
    Handle<Expression> ParseIfStatement();
    Handle<Expression> ParseElseBranch();
    Handle<Expression> ParseContinueStatement();
    Handle<Expression> ParseBreakStatement();
    Handle<Expression> ParseReturnStatement();
    // Handle<Expression> ParseWithStatement();
    // Handle<Expression> ParseLabelledStatement();
    Handle<Expression> ParseSwitchStatement();
    Handle<Expression> ParseThrowStatement();
    Handle<Expression> ParseTryCatchStatement();
    // Handle<Expression> ParseDebuggerStatement();

    Handle<Expression> ParseForStatement();
    Handle<Expression> ParseWhileStatement();
    Handle<Expression> ParseDoWhileStatement();

    Handle<Expression> ParseFunctionStatement();
    std::vector<std::string> ParseParameterList();
    Handle<FunctionPrototype> ParseFunctionPrototype();

    Handle<ExpressionList> ParseCaseBlock();
    Handle<Expression> ParseDefaultClause();

    Handle<Expression> ParseExpression();
    Handle<Expression> ParseExpressionOptional();

    Handle<Declaration> ParseDeclaration();

    Handle<Expression> ParseProgram();

    Handle<Expression> ParseForInStatement(Handle<Expression> inexpr);

    Handle<Expression> ParseVariableOrExpressionOptional();

private:
    String GetStringLiteral();
    String GetIdentifierName();
    double ParseNumber(const Token &token);

    TokenType peek();
    void advance(bool not_regex = false);

    ASTBuilder* builder() { return builder_; }
    Tokenizer* lex() { return lex_; }
    ParserContext *context() { return ctx_; }
    ScopeManager *scope_manager() { return manager_; }

private:
    ParserContext *ctx_;
    ASTBuilder *builder_;
    Tokenizer *lex_;
    ScopeManager *manager_;
    ParserFlags flags_;
};

extern Handle<Expression> ParseProgram(Parser *parser);

}

#endif
