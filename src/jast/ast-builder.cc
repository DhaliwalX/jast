#include "jast/ast-builder.h"

namespace jast {

ExpressionList *ASTBuilder::NewExpressionList()
{
    return factory()->NewExpressionList();
}

Expression *ASTBuilder::NewNullLiteral()
{
    return factory()->NewNullLiteral(locator()->loc());
}

Expression *ASTBuilder::NewUndefinedLiteral()
{
    return factory()->NewUndefinedLiteral(locator()->loc());
}

Expression *ASTBuilder::NewThisHolder()
{
    return factory()->NewThisHolder(locator()->loc());
}

Expression *ASTBuilder::NewIntegralLiteral(double value)
{
    return factory()->NewIntegralLiteral(locator()->loc(), value);
}

Expression *ASTBuilder::NewStringLiteral(const std::string &str)
{
    return factory()->NewStringLiteral(locator()->loc(), str);
}

Expression *ASTBuilder::NewRegExpLiteral(const std::string &str, const std::vector<RegExpFlags> &flags)
{
    return factory()->NewRegExpLiteral(locator()->loc(), str, flags);
}

Expression *ASTBuilder::NewTemplateLiteral(const std::string &str)
{
    return factory()->NewTemplateLiteral(locator()->loc(), str);
}

Expression *ASTBuilder::NewArrayLiteral(ProxyArray arr)
{
    return factory()->NewArrayLiteral(locator()->loc(), std::move(arr));
}

Expression *ASTBuilder::NewObjectLiteral(ProxyObject obj)
{
    return factory()->NewObjectLiteral(locator()->loc(), std::move(obj));
}

Expression *ASTBuilder::NewIdentifier(std::string name)
{
    return factory()->NewIdentifier(locator()->loc(), name);
}

Expression *ASTBuilder::NewBooleanLiteral(bool value)
{
    return factory()->NewBooleanLiteral(locator()->loc(), value);
}

Expression *ASTBuilder::NewArgumentList(ExpressionList *args)
{
    return factory()->NewArgumentList(locator()->loc(), args);
}

Expression *ASTBuilder::NewCallExpression(MemberAccessKind kind,
    Expression *func, Expression *args)
{
    return factory()->NewCallExpression(locator()->loc(), kind, func, args);
}

Expression *ASTBuilder::NewMemberExpression(MemberAccessKind kind,
    Expression *func, Expression *args)
{
    return factory()->NewMemberExpression(locator()->loc(), kind, func, args);
}

Expression *ASTBuilder::NewNewExpression(Expression *expr)
{
    return factory()->NewNewExpression(locator()->loc(), expr);
}

Expression *ASTBuilder::NewPrefixExpression(PrefixOperation op,
    Expression *expr)
{
    return factory()->NewPrefixExpression(locator()->loc(), op, expr);
}

Expression *ASTBuilder::NewPostfixExpression(PostfixOperation op,
    Expression *expr)
{
    return factory()->NewPostfixExpression(locator()->loc(), op, expr);
}

Expression *ASTBuilder::NewBinaryExpression(BinaryOperation op,
    Expression *lhs, Expression *rhs)
{
    return factory()->NewBinaryExpression(locator()->loc(), op, lhs, rhs);
}

Expression *ASTBuilder::NewAssignExpression(Expression *lhs, Expression *rhs)
{
    return factory()->NewAssignExpression(locator()->loc(), lhs, rhs);
}

Expression *ASTBuilder::NewTernaryExpression(Expression *first,
    Expression *second, Expression *third)
{
    return factory()->NewTernaryExpression(locator()->loc(),
        first, second, third);
}

Expression *ASTBuilder::NewCommaExpression(ExpressionList *list)
{
    return factory()->NewCommaExpression(locator()->loc(), list);
}

Expression *ASTBuilder::NewBlockStatement(ExpressionList *stmts)
{
    return factory()->NewBlockStatement(locator()->loc(), stmts);
}

Expression *ASTBuilder::NewForStatement(ForKind kind, Expression *init,
    Expression *cond, Expression *update, Expression *body)
{
    return factory()->NewForStatement(locator()->loc(), kind, init,
        cond, update, body);
}

Expression *ASTBuilder::NewWhileStatement(Expression *condition,
    Expression *body)
{
    return factory()->NewWhileStatement(locator()->loc(), condition, body);
}

Expression *ASTBuilder::NewDoWhileStatement(Expression *condition,
    Expression *body)
{
    return factory()->NewDoWhileStatement(locator()->loc(), condition, body);
}

Expression *ASTBuilder::NewFunctionPrototype(std::string name,
    std::vector<std::string> args)
{
    return factory()->NewFunctionPrototype(locator()->loc(), name,
        std::move(args));
}

Expression *ASTBuilder::NewFunctionStatement(FunctionPrototype *proto,
    Expression *body)
{
    return factory()->NewFunctionStatement(locator()->loc(), proto, body);
}

Expression *ASTBuilder::NewIfStatement(Expression *condition, Expression *then)
{
    return factory()->NewIfStatement(locator()->loc(), condition, then);
}

Expression *ASTBuilder::NewIfElseStatement(Expression *condition,
    Expression *then, Expression *els)
{
    return factory()->NewIfElseStatement(locator()->loc(), condition, then, els);
}

Expression *ASTBuilder::NewReturnStatement(Expression *expr)
{
    return factory()->NewReturnStatement(locator()->loc(), expr);
}

Expression *ASTBuilder::NewTryCatchStatement(Expression *try_block,
     Expression *catch_expr, Expression *catch_block, Expression *finally)
{
    return factory()->NewTryCatchStatement(locator()->loc(), try_block,
        catch_expr, catch_block, finally);
}

Expression *ASTBuilder::NewBreakStatement(Expression *label)
{
    return factory()->NewBreakStatement(locator()->loc(), label);
}

Expression *ASTBuilder::NewContinueStatement(Expression *label)
{
    return factory()->NewContinueStatement(locator()->loc(), label);
}

Expression *ASTBuilder::NewLabelledStatement(std::string label, Expression *expr)
{
    return factory()->NewLabelledStatement(locator()->loc(), label, expr);
}

Expression *ASTBuilder::NewCaseClauseStatement(Expression *clause,
    Expression *stmt)
{
    return factory()->NewCaseClauseStatement(locator()->loc(), clause, stmt);
}

ClausesList *ASTBuilder::NewClausesList()
{
    return factory()->NewClausesList();
}

Expression *ASTBuilder::NewSwitchStatement(Expression *expr,
    ClausesList *clauses)
{
    return factory()->NewSwitchStatement(locator()->loc(), expr, clauses);
}

Expression *ASTBuilder::NewThrowStatement(Expression *expr)
{
    return factory()->NewThrowStatement(locator()->loc(), expr);
}

}

