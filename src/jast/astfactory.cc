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

ExpressionList* ASTFactory::NewExpressionList()
{
    return new ExpressionList();
}

Expression* ASTFactory::NewNullLiteral(Position &loc)
{
    return new NullLiteral(loc);
}

Expression* ASTFactory::NewUndefinedLiteral(Position &loc)
{
    return new UndefinedLiteral(loc);
}

Expression* ASTFactory::NewThisHolder(Position &loc)
{
    return new ThisHolder(loc);
}

Expression* ASTFactory::NewIntegralLiteral(Position &loc,
    double value)
{
    return new IntegralLiteral(loc, value);
}

Expression* ASTFactory::NewStringLiteral(Position &loc,
    std::string str)
{
    return new StringLiteral(loc, str);
}

Expression* ASTFactory::NewTemplateLiteral(Position &loc,
    std::string str)
{
    return new TemplateLiteral(loc, str);
}

Expression* ASTFactory::NewArrayLiteral(Position &loc, ProxyArray arr)
{
    return new ArrayLiteral(loc, std::move(arr));
}

Expression* ASTFactory::NewObjectLiteral(Position &loc,
    ProxyObject obj)
{
    return new ObjectLiteral(loc, std::move(obj));
}

Expression* ASTFactory::NewIdentifier(Position &loc, std::string name)
{
    return new Identifier(loc, name);
}

Expression* ASTFactory::NewBooleanLiteral(Position &loc, bool val)
{
    return new BooleanLiteral(loc, val);
}

Expression* ASTFactory::NewArgumentList(Position &loc, ExpressionList* args)
{
    return new ArgumentList(loc, std::move(args));
}

Expression* ASTFactory::NewCallExpression(Position &loc,
    MemberAccessKind kind, Expression *func, Expression *args)
{
    return new CallExpression(loc, kind, func, args);
}

Expression *ASTFactory::NewMemberExpression(Position &loc,
    MemberAccessKind kind, Expression *expr, Expression *mem)
{
    return new MemberExpression(loc, kind, expr, mem);
}

Expression *ASTFactory::NewNewExpression(Position &loc, Expression *expr)
{
    return new NewExpression(loc, expr);
}

Expression *ASTFactory::NewPrefixExpression(Position &loc,
    PrefixOperation op, Expression *expr)
{
    return new PrefixExpression(loc, op, expr);
}

Expression *ASTFactory::NewPostfixExpression(Position &loc,
    PostfixOperation op, Expression *expr)
{
    return new PostfixExpression(loc, op, expr);
}

Expression *ASTFactory::NewBinaryExpression(Position &loc,
    BinaryOperation op, Expression *lhs, Expression *rhs)
{
    return new BinaryExpression(loc, op, lhs, rhs);
}

Expression *ASTFactory::NewAssignExpression(Position &loc,
    Expression *lhs, Expression *rhs)
{
    return new AssignExpression(loc, lhs, rhs);
}

Expression *ASTFactory::NewTernaryExpression(Position &loc,
    Expression *first, Expression *second, Expression *third)
{
    return new TernaryExpression(loc, first, second, third);
}

Expression *ASTFactory::NewCommaExpression(Position &loc,
    ExpressionList *l)
{
    return new CommaExpression(loc, l);
}

Declaration *ASTFactory::NewDeclaration(Position &loc, std::string name,
    Expression *init)
{
    return new Declaration(loc, name, init);
}

Expression *ASTFactory::NewDeclarationList(Position &loc,
    std::vector<std::unique_ptr<Declaration>> decls)
{
    return new DeclarationList(loc, std::move(decls));
}

Expression *ASTFactory::NewDeclarationList(Position &loc)
{
    return new DeclarationList(loc, {});
}

Expression *ASTFactory::NewBlockStatement(Position &loc,
                                            ExpressionList *list)
{
    return new BlockStatement(loc, list);
}

Expression *ASTFactory::NewForStatement(Position &loc, ForKind kind,
        Expression *init, Expression *condition, Expression *update,
        Expression *body)
{
    return new ForStatement(loc, kind, init, condition, update, body);
}

Expression *ASTFactory::NewWhileStatement(Position &loc,
    Expression *condition, Expression *body)
{
    return new WhileStatement(loc, condition, body);
}

Expression *ASTFactory::NewDoWhileStatement(Position &loc,
    Expression *condition, Expression *body)
{
    return new DoWhileStatement(loc, condition, body);
}

Expression *ASTFactory::NewFunctionPrototype(Position &loc,
    std::string name, std::vector<std::string> args)
{
    return new FunctionPrototype(loc, name, std::move(args));
}

Expression *ASTFactory::NewFunctionStatement(Position &loc,
    FunctionPrototype *proto, Expression *body)
{
    return new FunctionStatement(loc, proto, body);
}

Expression *ASTFactory::NewIfStatement(Position &loc,
    Expression *condition, Expression *then)
{
    return new IfStatement(loc, condition, then);
}

Expression *ASTFactory::NewIfElseStatement(Position &loc,
    Expression *condition, Expression *then, Expression *els)
{
    return new IfElseStatement(loc, condition, then, els);
}

Expression *ASTFactory::NewReturnStatement(Position &loc,
    Expression *expr)
{
    return new ReturnStatement(loc, expr);
}

Expression *ASTFactory::NewTryCatchStatement(Position &loc, Expression *tb,
        Expression *catch_expr, Expression *catch_block, Expression *finally)
{
    return new TryCatchStatement(loc, tb, catch_expr, catch_block, finally);
}

Expression *ASTFactory::NewBreakStatement(Position &loc, Expression *label)
{
    return new BreakStatement(loc, label);
}

Expression *ASTFactory::NewContinueStatement(Position &loc,
    Expression *label)
{
    return new ContinueStatement(loc, label);
}

Expression *ASTFactory::NewLabelledStatement(Position &loc,
    std::string label, Expression *stmt)
{
    return new LabelledStatement(loc, label, stmt);
}

Expression *ASTFactory::NewCaseClauseStatement(Position &loc,
        Expression *clause, Expression *stmt)
{
    return new CaseClauseStatement(loc, clause, stmt);
}

ClausesList *ASTFactory::NewClausesList()
{
    return new ClausesList();
}

Expression *ASTFactory::NewSwitchStatement(Position &loc,
        Expression *expr, ClausesList *clauses)
{
    return new SwitchStatement(loc, expr, clauses);
}

Expression *ASTFactory::NewThrowStatement(Position &loc,
    Expression *expr)
{
    return new ThrowStatement(loc, expr);
}

}
