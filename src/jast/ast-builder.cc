#include "jast/ast-builder.h"

namespace jast {

Handle<ExpressionList> ASTBuilder::NewExpressionList()
{
    return factory()->NewExpressionList();
}

Handle<Expression> ASTBuilder::NewNullLiteral()
{
    return factory()->NewNullLiteral(locator()->loc(), manager()->current());
}

Handle<Expression> ASTBuilder::NewUndefinedLiteral()
{
    return factory()->NewUndefinedLiteral(locator()->loc(), manager()->current());
}

Handle<Expression> ASTBuilder::NewThisHolder()
{
    return factory()->NewThisHolder(locator()->loc(), manager()->current());
}

Handle<Expression> ASTBuilder::NewIntegralLiteral(double value)
{
    return factory()->NewIntegralLiteral(locator()->loc(), manager()->current(), value);
}

Handle<Expression> ASTBuilder::NewStringLiteral(const std::string &str)
{
    return factory()->NewStringLiteral(locator()->loc(), manager()->current(), str);
}

Handle<Expression> ASTBuilder::NewRegExpLiteral(const std::string &str, const std::vector<RegExpFlags> &flags)
{
    return factory()->NewRegExpLiteral(locator()->loc(), manager()->current(), str, flags);
}

Handle<Expression> ASTBuilder::NewTemplateLiteral(const std::string &str)
{
    return factory()->NewTemplateLiteral(locator()->loc(), manager()->current(), str);
}

Handle<Expression> ASTBuilder::NewArrayLiteral(ProxyArray arr)
{
    return factory()->NewArrayLiteral(locator()->loc(), manager()->current(), std::move(arr));
}

Handle<Expression> ASTBuilder::NewObjectLiteral(ProxyObject obj)
{
    return factory()->NewObjectLiteral(locator()->loc(), manager()->current(), std::move(obj));
}

Handle<Expression> ASTBuilder::NewIdentifier(std::string name)
{
    return factory()->NewIdentifier(locator()->loc(), manager()->current(), name);
}

Handle<Expression> ASTBuilder::NewBooleanLiteral(bool value)
{
    return factory()->NewBooleanLiteral(locator()->loc(), manager()->current(), value);
}

Handle<Expression> ASTBuilder::NewArgumentList(Handle<ExpressionList> args)
{
    return factory()->NewArgumentList(locator()->loc(), manager()->current(), args);
}

Handle<Expression> ASTBuilder::NewCallExpression(MemberAccessKind kind,
    Handle<Expression> func, Handle<Expression> args)
{
    return factory()->NewCallExpression(locator()->loc(), manager()->current(), kind, func, args);
}

Handle<Expression> ASTBuilder::NewMemberExpression(MemberAccessKind kind,
    Handle<Expression> func, Handle<Expression> args)
{
    return factory()->NewMemberExpression(locator()->loc(), manager()->current(), kind, func, args);
}

Handle<Expression> ASTBuilder::NewNewExpression(Handle<Expression> expr)
{
    return factory()->NewNewExpression(locator()->loc(), manager()->current(), expr);
}

Handle<Expression> ASTBuilder::NewPrefixExpression(PrefixOperation op,
    Handle<Expression> expr)
{
    return factory()->NewPrefixExpression(locator()->loc(), manager()->current(), op, expr);
}

Handle<Expression> ASTBuilder::NewPostfixExpression(PostfixOperation op,
    Handle<Expression> expr)
{
    return factory()->NewPostfixExpression(locator()->loc(), manager()->current(), op, expr);
}

Handle<Expression> ASTBuilder::NewBinaryExpression(BinaryOperation op,
    Handle<Expression> lhs, Handle<Expression> rhs)
{
    return factory()->NewBinaryExpression(locator()->loc(), manager()->current(), op, lhs, rhs);
}

Handle<Expression> ASTBuilder::NewAssignExpression(Handle<Expression> lhs, Handle<Expression> rhs)
{
    return factory()->NewAssignExpression(locator()->loc(), manager()->current(), lhs, rhs);
}

Handle<Expression> ASTBuilder::NewTernaryExpression(Handle<Expression> first,
    Handle<Expression> second, Handle<Expression> third)
{
    return factory()->NewTernaryExpression(locator()->loc(), manager()->current(),
        first, second, third);
}

Handle<Expression> ASTBuilder::NewCommaExpression(Handle<ExpressionList> list)
{
    return factory()->NewCommaExpression(locator()->loc(), manager()->current(), list);
}

Handle<Expression> ASTBuilder::NewBlockStatement(Handle<ExpressionList> stmts)
{
    return factory()->NewBlockStatement(locator()->loc(), manager()->current(), stmts);
}

Handle<Expression> ASTBuilder::NewForStatement(ForKind kind, Handle<Expression> init,
    Handle<Expression> cond, Handle<Expression> update, Handle<Expression> body)
{
    return factory()->NewForStatement(locator()->loc(), manager()->current(), kind, init,
        cond, update, body);
}

Handle<Expression> ASTBuilder::NewWhileStatement(Handle<Expression> condition,
    Handle<Expression> body)
{
    return factory()->NewWhileStatement(locator()->loc(), manager()->current(), condition, body);
}

Handle<Expression> ASTBuilder::NewDoWhileStatement(Handle<Expression> condition,
    Handle<Expression> body)
{
    return factory()->NewDoWhileStatement(locator()->loc(), manager()->current(), condition, body);
}

Handle<Expression> ASTBuilder::NewFunctionPrototype(std::string name,
    std::vector<std::string> args)
{
    return factory()->NewFunctionPrototype(locator()->loc(), manager()->current(), name,
        std::move(args));
}

Handle<Expression> ASTBuilder::NewFunctionStatement(Handle<FunctionPrototype> proto,
    Handle<Expression> body)
{
    return factory()->NewFunctionStatement(locator()->loc(), manager()->current(), proto, body);
}

Handle<Expression> ASTBuilder::NewIfStatement(Handle<Expression> condition, Handle<Expression> then)
{
    return factory()->NewIfStatement(locator()->loc(), manager()->current(), condition, then);
}

Handle<Expression> ASTBuilder::NewIfElseStatement(Handle<Expression> condition,
    Handle<Expression> then, Handle<Expression> els)
{
    return factory()->NewIfElseStatement(locator()->loc(), manager()->current(), condition, then, els);
}

Handle<Expression> ASTBuilder::NewReturnStatement(Handle<Expression> expr)
{
    return factory()->NewReturnStatement(locator()->loc(), manager()->current(), expr);
}

Handle<Expression> ASTBuilder::NewTryCatchStatement(Handle<Expression> try_block,
     Handle<Expression> catch_expr, Handle<Expression> catch_block, Handle<Expression> finally)
{
    return factory()->NewTryCatchStatement(locator()->loc(), manager()->current(), try_block,
        catch_expr, catch_block, finally);
}

Handle<Expression> ASTBuilder::NewBreakStatement(Handle<Expression> label)
{
    return factory()->NewBreakStatement(locator()->loc(), manager()->current(), label);
}

Handle<Expression> ASTBuilder::NewContinueStatement(Handle<Expression> label)
{
    return factory()->NewContinueStatement(locator()->loc(), manager()->current(), label);
}

Handle<Expression> ASTBuilder::NewLabelledStatement(std::string label, Handle<Expression> expr)
{
    return factory()->NewLabelledStatement(locator()->loc(), manager()->current(), label, expr);
}

Handle<Expression> ASTBuilder::NewCaseClauseStatement(Handle<Expression> clause,
    Handle<Expression> stmt)
{
    return factory()->NewCaseClauseStatement(locator()->loc(), manager()->current(), clause, stmt);
}

Handle<ClausesList> ASTBuilder::NewClausesList()
{
    return factory()->NewClausesList();
}

Handle<Expression> ASTBuilder::NewSwitchStatement(Handle<Expression> expr,
    Handle<ClausesList> clauses)
{
    return factory()->NewSwitchStatement(locator()->loc(), manager()->current(), expr, clauses);
}

Handle<Expression> ASTBuilder::NewThrowStatement(Handle<Expression> expr)
{
    return factory()->NewThrowStatement(locator()->loc(), manager()->current(), expr);
}

}

