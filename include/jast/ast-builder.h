#ifndef AST_BUILDER_H_
#define AST_BUILDER_H_

#include "jast/astfactory.h"
#include "jast/source-locator.h"
#include "jast/context.h"
#include "jast/expression.h"
#include "jast/scope.h"
#include <list>

namespace jast {

// ASTBuilder ::= helper class for building the AST in a safe
// and better way. This class can do constant folding on the go. Thus producing
// shorter AST
class ASTBuilder {
public:
    ASTBuilder(ParserContext *ctx, ASTFactory *factory,
            SourceLocator *locator, ScopeManager *manager)
        : factory_{ factory }, locator_{ locator }, ctx_{ ctx }, manager_{manager}
    { }

    // creates a heap allocated expression list and return a pointer to it.
    // Helpful for expression which requires list of expression
    Handle<ExpressionList> NewExpressionList();

    // create a new node representing JavaScript 'null'
    Handle<Expression> NewNullLiteral();

    // create a new node representing undefined
    Handle<Expression> NewUndefinedLiteral();

    // create a new node representing JavaScript 'this'
    Handle<Expression> NewThisHolder();

    // create a new node representing JavaScript number, for our purposes
    // double suffices our need
    Handle<Expression> NewIntegralLiteral(double value);

    // create a new node representing JavaScript string
    Handle<Expression> NewStringLiteral(const std::string &str);

    Handle<Expression> NewTemplateLiteral(const std::string &str);

    Handle<Expression> NewRegExpLiteral(const std::string &str, const std::vector<RegExpFlags> &flags);

    // create a new node representing JavaScript array
    // after passing `arr` to this function, your arr becomes unusable
    Handle<Expression> NewArrayLiteral(ProxyArray arr);

    // create a new node representing JavaScript object
    Handle<Expression> NewObjectLiteral(ProxyObject obj);

    // create a new node representing JavaScript Identifier
    Handle<Expression> NewIdentifier(std::string name);

    // create a new node representing JavaScript boolean
    Handle<Expression> NewBooleanLiteral(bool value);

    // create a new node representing JavaScript argument list
    Handle<Expression> NewArgumentList(Handle<ExpressionList> arg);

    // create a new node representing JavaScript member call
    Handle<Expression> NewCallExpression(MemberAccessKind kind, Handle<Expression> func,
        Handle<Expression> args);

    // create a new node representing JavaScript member expression
    Handle<Expression> NewMemberExpression(MemberAccessKind kind, Handle<Expression> expr,
        Handle<Expression> args);

    // create a new node representing JavaScript `new` expression
    Handle<Expression> NewNewExpression(Handle<Expression> expr);

    // create a new node representing JavaScript prefix operation
    Handle<Expression> NewPrefixExpression(PrefixOperation op, Handle<Expression> expr);

    // create a new node representing JavaScript postfix operation
    Handle<Expression> NewPostfixExpression(PostfixOperation op, Handle<Expression> expr);

    // create a new node representing JavaScript binary expression
    Handle<Expression> NewBinaryExpression(BinaryOperation op, Handle<Expression> lhs,
        Handle<Expression> rhs);

    // create a new node representing JavaScript assign expression
    Handle<Expression> NewAssignExpression(Handle<Expression> lhs, Handle<Expression> rhs);

    // create a new node representing JavaScript ternary expression
    Handle<Expression> NewTernaryExpression(Handle<Expression> first, Handle<Expression> second,
        Handle<Expression> third);

    // create a new node representing JavaScript Comma Expression
    Handle<Expression> NewCommaExpression(Handle<ExpressionList> list);

    // create a new node representing JavaScript block statement
    Handle<Expression> NewBlockStatement(Handle<ExpressionList> stmts);
    // create a new node representing JavaScript for loop
    Handle<Expression> NewForStatement(ForKind kind, Handle<Expression> init, Handle<Expression> cond,
        Handle<Expression> update, Handle<Expression> body);

    // create a new node representing JavaScript while loop
    Handle<Expression> NewWhileStatement(Handle<Expression> condition, Handle<Expression> body);

    // create a new node representing JavaScript do - while loop
    Handle<Expression> NewDoWhileStatement(Handle<Expression> condition, Handle<Expression> body);

    // create a new node representing JavaScript FunctionPrototype
    Handle<Expression> NewFunctionPrototype(std::string name,
        std::vector<std::string> args);

    // create a new node representing JavaScript function statement
    // and fuunction expression
    Handle<Expression> NewFunctionStatement(Handle<FunctionPrototype> proto, Handle<Expression> body);

    // create a new node representing JavaScript if statement
    Handle<Expression> NewIfStatement(Handle<Expression> condition, Handle<Expression> then);

    // create a new node representing JavaScript if else statement
    Handle<Expression> NewIfElseStatement(Handle<Expression> cond, Handle<Expression> then,
        Handle<Expression> els);

    // create a new node representing JavaScript return statement
    Handle<Expression> NewReturnStatement(Handle<Expression> ret);

    // create a new node representing JavaScript try-catch
    Handle<Expression> NewTryCatchStatement(Handle<Expression> try_block,
        Handle<Expression> catch_expr, Handle<Expression> catch_block, Handle<Expression> finally);

    // create a new node representing JavaScript break statement
    Handle<Expression> NewBreakStatement(Handle<Expression> label = nullptr);

    // create a new node representing JavaScript continue statement
    Handle<Expression> NewContinueStatement(Handle<Expression> label = nullptr);

    // create a new node representing JavaScript label statement
    Handle<Expression> NewLabelledStatement(std::string label, Handle<Expression> expr);

    // create a new node representing JavaScript case clause
    Handle<Expression> NewCaseClauseStatement(Handle<Expression> clause, Handle<Expression> stmt);

    // create a JavaScript case list
    Handle<ClausesList> NewClausesList();

    // create a new node representing switch statement
    Handle<Expression> NewSwitchStatement(Handle<Expression> expr, Handle<ClausesList> clauses);

    // create a new node representing throw statement
    Handle<Expression> NewThrowStatement(Handle<Expression> expr);
    ASTFactory *factory() { return factory_; }
    SourceLocator *locator() { return locator_; }
    ScopeManager *manager() { return manager_; }

    template <typename T>
    inline Handle<Expression> save(Handle<T> handle) {
        exprs_.push_back(handle);
        return handle;
    }
private:
    ASTFactory *factory_;
    SourceLocator *locator_;
    ParserContext *ctx_;
    ScopeManager *manager_;
    std::list<Handle<Expression>> exprs_;
};

}

#endif
