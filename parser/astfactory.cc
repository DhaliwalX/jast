#include "parser/astfactory.h"

namespace grok {
namespace parser {

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

Expression* ASTFactory::NewNullLiteral(SourceLocation &loc)
{
    return new NullLiteral(loc);
}

Expression* ASTFactory::NewUndefinedLiteral(SourceLocation &loc)
{
    return new UndefinedLiteral(loc);
}

Expression* ASTFactory::NewThisHolder(SourceLocation &loc)
{
    return new ThisHolder(loc);
}

Expression* ASTFactory::NewIntegralLiteral(SourceLocation &loc,
    double value)
{
    return new IntegralLiteral(loc, value);
}

Expression* ASTFactory::NewStringLiteral(SourceLocation &loc,
    std::string str)
{
    return new StringLiteral(loc, str);
}

Expression* ASTFactory::NewArrayLiteral(SourceLocation &loc, ProxyArray arr)
{
    return new ArrayLiteral(loc, std::move(arr));
}

Expression* ASTFactory::NewObjectLiteral(SourceLocation &loc,
    ProxyObject obj)
{
    return new ObjectLiteral(loc, std::move(obj));
}

Expression* ASTFactory::NewIdentifier(SourceLocation &loc, std::string name)
{
    return new Identifier(loc, name);
}

Expression* ASTFactory::NewBooleanLiteral(SourceLocation &loc, bool val)
{
    return new BooleanLiteral(loc, val);
}

Expression* ASTFactory::NewArgumentList(SourceLocation &loc, ExpressionList* args)
{
    return new ArgumentList(loc, std::move(args));
}

Expression* ASTFactory::NewCallExpression(SourceLocation &loc, 
    MemberAccessKind kind, Expression *func, Expression *args)
{
    return new CallExpression(loc, kind, func, args);
}

Expression *ASTFactory::NewMemberExpression(SourceLocation &loc,
    MemberAccessKind kind, Expression *expr, Expression *mem)
{
    return new MemberExpression(loc, kind, expr, mem);
}

Expression *ASTFactory::NewNewExpression(SourceLocation &loc, Expression *expr)
{
    return new NewExpression(loc, expr);
}

Expression *ASTFactory::NewPrefixExpression(SourceLocation &loc,
    PrefixOperation op, Expression *expr)
{
    return new PrefixExpression(loc, op, expr);
}

Expression *ASTFactory::NewPostfixExpression(SourceLocation &loc,
    PostfixOperation op, Expression *expr)
{
    return new PostfixExpression(loc, op, expr);
}

Expression *ASTFactory::NewBinaryExpression(SourceLocation &loc,
    BinaryOperation op, Expression *lhs, Expression *rhs)
{
    return new BinaryExpression(loc, op, lhs, rhs);
}

Expression *ASTFactory::NewAssignExpression(SourceLocation &loc,
    Expression *lhs, Expression *rhs)
{
    return new AssignExpression(loc, lhs, rhs);
}

Expression *ASTFactory::NewTernaryExpression(SourceLocation &loc,
    Expression *first, Expression *second, Expression *third)
{
    return new TernaryExpression(loc, first, second, third);
}

Expression *ASTFactory::NewCommaExpression(SourceLocation &loc,
    ExpressionList *l)
{
    return new CommaExpression(loc, l);
}

Declaration *ASTFactory::NewDeclaration(SourceLocation &loc, std::string name,
    Expression *init)
{
    return new Declaration(loc, name, init);
}

Expression *ASTFactory::NewDeclarationList(SourceLocation &loc,
    std::vector<std::unique_ptr<Declaration>> decls)
{
    return new DeclarationList(loc, std::move(decls));
}

Expression *ASTFactory::NewDeclarationList(SourceLocation &loc)
{
    return new DeclarationList(loc, {});
}

Expression *ASTFactory::NewBlockStatement(SourceLocation &loc,
                                            ExpressionList *list)
{
    return new BlockStatement(loc, list);
}

Expression *ASTFactory::NewForStatement(SourceLocation &loc, ForKind kind,
        Expression *init, Expression *condition, Expression *update,
        Expression *body)
{
    return new ForStatement(loc, kind, init, condition, update, body);
}

Expression *ASTFactory::NewWhileStatement(SourceLocation &loc,
    Expression *condition, Expression *body)
{
    return new WhileStatement(loc, condition, body);
}

Expression *ASTFactory::NewDoWhileStatement(SourceLocation &loc,
    Expression *condition, Expression *body)
{
    return new DoWhileStatement(loc, condition, body);
}

Expression *ASTFactory::NewFunctionPrototype(SourceLocation &loc,
    std::string name, std::vector<std::string> args)
{
    return new FunctionPrototype(loc, name, std::move(args));
}

Expression *ASTFactory::NewFunctionStatement(SourceLocation &loc,
    FunctionPrototype *proto, Expression *body)
{
    return new FunctionStatement(loc, proto, body);
}

Expression *ASTFactory::NewIfStatement(SourceLocation &loc,
    Expression *condition, Expression *then)
{
    return new IfStatement(loc, condition, then);
}

Expression *ASTFactory::NewIfElseStatement(SourceLocation &loc,
    Expression *condition, Expression *then, Expression *els)
{
    return new IfElseStatement(loc, condition, then, els);
}

Expression *ASTFactory::NewReturnStatement(SourceLocation &loc,
    Expression *expr)
{
    return new ReturnStatement(loc, expr);
}

Expression *ASTFactory::NewTryCatchStatement(SourceLocation &loc, Expression *tb,
        Expression *catch_expr, Expression *catch_block, Expression *finally)
{
    return new TryCatchStatement(loc, tb, catch_expr, catch_block, finally);
}

Expression *ASTFactory::NewBreakStatement(SourceLocation &loc, Expression *label)
{
    return new BreakStatement(loc, label);
}

Expression *ASTFactory::NewContinueStatement(SourceLocation &loc,
    Expression *label)
{
    return new ContinueStatement(loc, label);
}

Expression *ASTFactory::NewLabelledStatement(SourceLocation &loc,
    std::string label, Expression *stmt)
{
    return new LabelledStatement(loc, label, stmt);
}

Expression *ASTFactory::NewCaseClauseStatement(SourceLocation &loc,
        Expression *clause, Expression *stmt)
{
    return new CaseClauseStatement(loc, clause, stmt);
}

ClausesList *ASTFactory::NewClausesList()
{
    return new ClausesList();
}

Expression *ASTFactory::NewSwitchStatement(SourceLocation &loc,
        Expression *expr, ClausesList *clauses)
{
    return new SwitchStatement(loc, expr, clauses);
}

}
}
