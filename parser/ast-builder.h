#ifndef AST_BUILDER_H_
#define AST_BUILDER_H_

#include "parser/astfactory.h"
#include "parser/source-locator.h"
#include "parser/context.h"

namespace grok {
namespace parser {

// ASTBuilder ::= helper class for building the AST in a safe
// and better way. This class can do constant folding on the go. Thus producing
// shorter AST
class ASTBuilder {
public:
    ASTBuilder(ParserContext *ctx, ASTFactory *factory, SourceLocator *locator)
        : factory_{ factory }, locator_{ locator }, ctx_{ ctx }
    { }

    // creates a heap allocated expression list and return a pointer to it.
    // Helpful for expression which requires list of expression
    ExpressionList *NewExpressionList();

    // create a new node representing JavaScript 'null'
    Expression *NewNullLiteral();

    // create a new node representing undefined
    Expression *NewUndefinedLiteral();

    // create a new node representing JavaScript 'this'
    Expression *NewThisHolder();

    // create a new node representing JavaScript number, for our purposes
    // double suffices our need
    Expression *NewIntegralLiteral(double value);

    // create a new node representing JavaScript string
    Expression *NewStringLiteral(const std::string &str);

    // create a new node representing JavaScript array
    // after passing `arr` to this function, your arr becomes unusable 
    Expression *NewArrayLiteral(ProxyArray arr);

    // create a new node representing JavaScript object
    Expression *NewObjectLiteral(ProxyObject obj);

    // create a new node representing JavaScript Identifier
    Expression *NewIdentifier(std::string name);

    // create a new node representing JavaScript boolean
    Expression *NewBooleanLiteral(bool value);

    // create a new node representing JavaScript argument list
    Expression *NewArgumentList(ExpressionList *arg);

    // create a new node representing JavaScript member call
    Expression *NewCallExpression(MemberAccessKind kind, Expression *func,
        Expression *args);

    // create a new node representing JavaScript member expression
    Expression *NewMemberExpression(MemberAccessKind kind, Expression *expr,
        Expression *args);

    // create a new node representing JavaScript `new` expression
    Expression *NewNewExpression(Expression *expr);

    // create a new node representing JavaScript prefix operation
    Expression *NewPrefixExpression(PrefixOperation op, Expression *expr);

    // create a new node representing JavaScript postfix operation
    Expression *NewPostfixExpression(PostfixOperation op, Expression *expr);

    // create a new node representing JavaScript binary expression
    Expression *NewBinaryExpression(BinaryOperation op, Expression *lhs,
        Expression *rhs);

    // create a new node representing JavaScript assign expression
    Expression *NewAssignExpression(Expression *lhs, Expression *rhs);

    // create a new node representing JavaScript ternary expression
    Expression *NewTernaryExpression(Expression *first, Expression *second,
        Expression *third);

    // create a new node representing JavaScript Comma Expression
    Expression *NewCommaExpression(ExpressionList *list);

    // create a new node representing JavaScript block statement
    Expression *NewBlockStatement(ExpressionList *stmts);
    // create a new node representing JavaScript for loop
    Expression *NewForStatement(ForKind kind, Expression *init, Expression *cond,
        Expression *update, Expression *body);

    // create a new node representing JavaScript while loop
    Expression *NewWhileStatement(Expression *condition, Expression *body);

    // create a new node representing JavaScript do - while loop
    Expression *NewDoWhileStatement(Expression *condition, Expression *body);

    // create a new node representing JavaScript FunctionPrototype
    Expression *NewFunctionPrototype(std::string name,
        std::vector<std::string> args);

    // create a new node representing JavaScript function statement
    // and fuunction expression
    Expression *NewFunctionStatement(FunctionPrototype *proto, Expression *body);

    // create a new node representing JavaScript if statement
    Expression *NewIfStatement(Expression *condition, Expression *then);

    // create a new node representing JavaScript if else statement
    Expression *NewIfElseStatement(Expression *cond, Expression *then,
        Expression *els);

    // create a new node representing JavaScript return statement
    Expression *NewReturnStatement(Expression *ret);

    // create a new node representing JavaScript try-catch
    Expression *NewTryCatchStatement(Expression *try_block,
        Expression *catch_expr, Expression *catch_block, Expression *finally);

    // create a new node representing JavaScript break statement
    Expression *NewBreakStatement(Expression *label = nullptr);

    // create a new node representing JavaScript continue statement
    Expression *NewContinueStatement(Expression *label = nullptr);

    // create a new node representing JavaScript label statement
    Expression *NewLabelledStatement(std::string label, Expression *expr);

    // create a new node representing JavaScript case clause
    Expression *NewCaseClauseStatement(Expression *clause, Expression *stmt);

    // create a JavaScript case list
    ClausesList *NewClausesList();

    // create a new node representing switch statement
    Expression *NewSwitchStatement(Expression *expr, ClausesList *clauses);

    ASTFactory *factory() { return factory_; }
    SourceLocator *locator() { return locator_; }

private:
    ASTFactory *factory_;
    SourceLocator *locator_;
    ParserContext *ctx_;
};

}
}

#endif
