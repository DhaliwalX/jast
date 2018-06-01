#include "jast/astfactory.h"

namespace jast {

// static
ASTFactory *ASTFactory::GetFactoryInstance()
{
    static std::unique_ptr<ASTFactory> factory_instance;

    if (!factory_instance) {
        factory_instance.reset(new ASTFactory());
    }
    return factory_instance.get();
}

Handle<ExpressionList> ASTFactory::NewExpressionList()
{
    return MakeHandle<ExpressionList>();
}

Handle<Expression> ASTFactory::NewNullLiteral(Position &loc, Scope *scope)
{
    return MakeHandle<NullLiteral>(loc, scope);
}

Handle<Expression> ASTFactory::NewUndefinedLiteral(Position &loc, Scope *scope)
{
    return MakeHandle<UndefinedLiteral>(loc, scope);
}

Handle<Expression> ASTFactory::NewThisHolder(Position &loc, Scope *scope)
{
    return MakeHandle<ThisHolder>(loc, scope);
}

Handle<Expression> ASTFactory::NewIntegralLiteral(Position &loc, Scope *scope,
    double value)
{
    return MakeHandle<IntegralLiteral>(loc, scope, value);
}

Handle<Expression> ASTFactory::NewStringLiteral(Position &loc, Scope *scope,
    std::string str)
{
    return MakeHandle<StringLiteral>(loc, scope, str);
}

Handle<Expression> ASTFactory::NewRegExpLiteral(Position &loc, Scope *scope,
    std::string str, const std::vector<RegExpFlags> &flags)
{
    return MakeHandle<RegExpLiteral>(loc, scope, str, flags);
}

Handle<Expression> ASTFactory::NewTemplateLiteral(Position &loc, Scope *scope,
    std::string str)
{
    return MakeHandle<TemplateLiteral>(loc, scope, str);
}

Handle<Expression> ASTFactory::NewArrayLiteral(Position &loc, Scope *scope, ProxyArray arr)
{
    return MakeHandle<ArrayLiteral>(loc, scope, std::move(arr));
}

Handle<Expression> ASTFactory::NewObjectLiteral(Position &loc, Scope *scope,
    ProxyObject obj)
{
    return MakeHandle<ObjectLiteral>(loc, scope, std::move(obj));
}

Handle<Expression> ASTFactory::NewIdentifier(Position &loc, Scope *scope, std::string name)
{
    return MakeHandle<Identifier>(loc, scope, name);
}

Handle<Expression> ASTFactory::NewBooleanLiteral(Position &loc, Scope *scope, bool val)
{
    return MakeHandle<BooleanLiteral>(loc, scope, val);
}

Handle<Expression> ASTFactory::NewArgumentList(Position &loc, Scope *scope, Handle<ExpressionList> args)
{
    return MakeHandle<ArgumentList>(loc, scope, std::move(args));
}

Handle<Expression> ASTFactory::NewCallExpression(Position &loc, Scope *scope,
    MemberAccessKind kind, Handle<Expression> func, Handle<Expression> args)
{
    return MakeHandle<CallExpression>(loc, scope, kind, func, args);
}

Handle<Expression> ASTFactory::NewMemberExpression(Position &loc, Scope *scope,
    MemberAccessKind kind, Handle<Expression> expr, Handle<Expression> mem)
{
    return MakeHandle<MemberExpression>(loc, scope, kind, expr, mem);
}

Handle<Expression> ASTFactory::NewNewExpression(Position &loc, Scope *scope, Handle<Expression> expr)
{
    return MakeHandle<NewExpression>(loc, scope, expr);
}

Handle<Expression> ASTFactory::NewPrefixExpression(Position &loc, Scope *scope,
    PrefixOperation op, Handle<Expression> expr)
{
    return MakeHandle<PrefixExpression>(loc, scope, op, expr);
}

Handle<Expression> ASTFactory::NewPostfixExpression(Position &loc, Scope *scope,
    PostfixOperation op, Handle<Expression> expr)
{
    return MakeHandle<PostfixExpression>(loc, scope, op, expr);
}

Handle<Expression> ASTFactory::NewBinaryExpression(Position &loc, Scope *scope,
    BinaryOperation op, Handle<Expression> lhs, Handle<Expression> rhs)
{
    return MakeHandle<BinaryExpression>(loc, scope, op, lhs, rhs);
}

Handle<Expression> ASTFactory::NewAssignExpression(Position &loc, Scope *scope,
    Handle<Expression> lhs, Handle<Expression> rhs)
{
    return MakeHandle<AssignExpression>(loc, scope, lhs, rhs);
}

Handle<Expression> ASTFactory::NewTernaryExpression(Position &loc, Scope *scope,
    Handle<Expression> first, Handle<Expression> second, Handle<Expression> third)
{
    return MakeHandle<TernaryExpression>(loc, scope, first, second, third);
}

Handle<Expression> ASTFactory::NewCommaExpression(Position &loc, Scope *scope,
    Handle<ExpressionList> l)
{
    return MakeHandle<CommaExpression>(loc, scope, l);
}

Handle<Declaration> ASTFactory::NewDeclaration(Position &loc, Scope *scope, std::string name,
    Handle<Expression> init)
{
    return MakeHandle<Declaration>(loc, scope, name, init);
}

Handle<Expression> ASTFactory::NewDeclarationList(Position &loc, Scope *scope,
    std::vector<Handle<Declaration>> decls)
{
    return MakeHandle<DeclarationList>(loc, scope, std::move(decls));
}

Handle<Expression> ASTFactory::NewDeclarationList(Position &loc, Scope *scope)
{
    return MakeHandle<DeclarationList>(loc, scope);
}

Handle<Expression> ASTFactory::NewBlockStatement(Position &loc, Scope *scope,
                                            Handle<ExpressionList> list)
{
    return MakeHandle<BlockStatement>(loc, scope, list);
}

Handle<Expression> ASTFactory::NewForStatement(Position &loc, Scope *scope, ForKind kind,
        Handle<Expression> init, Handle<Expression> condition, Handle<Expression> update,
        Handle<Expression> body)
{
    return MakeHandle<ForStatement>(loc, scope, kind, init, condition, update, body);
}

Handle<Expression> ASTFactory::NewWhileStatement(Position &loc, Scope *scope,
    Handle<Expression> condition, Handle<Expression> body)
{
    return MakeHandle<WhileStatement>(loc, scope, condition, body);
}

Handle<Expression> ASTFactory::NewDoWhileStatement(Position &loc, Scope *scope,
    Handle<Expression> condition, Handle<Expression> body)
{
    return MakeHandle<DoWhileStatement>(loc, scope, condition, body);
}

Handle<Expression> ASTFactory::NewFunctionPrototype(Position &loc, Scope *scope,
    std::string name, std::vector<std::string> args)
{
    return MakeHandle<FunctionPrototype>(loc, scope, name, std::move(args));
}

Handle<Expression> ASTFactory::NewFunctionStatement(Position &loc, Scope *scope,
    Handle<FunctionPrototype> proto, Handle<Expression> body)
{
    return MakeHandle<FunctionStatement>(loc, scope, proto, body);
}

Handle<Expression> ASTFactory::NewIfStatement(Position &loc, Scope *scope,
    Handle<Expression> condition, Handle<Expression> then)
{
    return MakeHandle<IfStatement>(loc, scope, condition, then);
}

Handle<Expression> ASTFactory::NewIfElseStatement(Position &loc, Scope *scope,
    Handle<Expression> condition, Handle<Expression> then, Handle<Expression> els)
{
    return MakeHandle<IfElseStatement>(loc, scope, condition, then, els);
}

Handle<Expression> ASTFactory::NewReturnStatement(Position &loc, Scope *scope,
    Handle<Expression> expr)
{
    return MakeHandle<ReturnStatement>(loc, scope, expr);
}

Handle<Expression> ASTFactory::NewTryCatchStatement(Position &loc, Scope *scope, Handle<Expression> tb,
        Handle<Expression> catch_expr, Handle<Expression> catch_block, Handle<Expression> finally)
{
    return MakeHandle<TryCatchStatement>(loc, scope, tb, catch_expr, catch_block, finally);
}

Handle<Expression> ASTFactory::NewBreakStatement(Position &loc, Scope *scope, Handle<Expression> label)
{
    return MakeHandle<BreakStatement>(loc, scope, label);
}

Handle<Expression> ASTFactory::NewContinueStatement(Position &loc, Scope *scope,
    Handle<Expression> label)
{
    return MakeHandle<ContinueStatement>(loc, scope, label);
}

Handle<Expression> ASTFactory::NewLabelledStatement(Position &loc, Scope *scope,
    std::string label, Handle<Expression> stmt)
{
    return MakeHandle<LabelledStatement>(loc, scope, label, stmt);
}

Handle<Expression> ASTFactory::NewCaseClauseStatement(Position &loc, Scope *scope,
        Handle<Expression> clause, Handle<Expression> stmt)
{
    return MakeHandle<CaseClauseStatement>(loc, scope, clause, stmt);
}

Handle<ClausesList> ASTFactory::NewClausesList()
{
    return MakeHandle<ClausesList>();
}

Handle<Expression> ASTFactory::NewSwitchStatement(Position &loc, Scope *scope,
        Handle<Expression> expr, Handle<ClausesList> clauses)
{
    return MakeHandle<SwitchStatement>(loc, scope, expr, clauses);
}

Handle<Expression> ASTFactory::NewThrowStatement(Position &loc, Scope *scope,
    Handle<Expression> expr)
{
    return MakeHandle<ThrowStatement>(loc, scope, expr);
}

}
