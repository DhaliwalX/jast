#ifndef ASTFACTORY_H_
#define ASTFACTORY_H_

#include "parser/expression.h"
#include "parser/statement.h"

namespace grok {
namespace parser {

// ASTFactory ::= factory for all the AST nodes it can be override'd so as
// to provide custom allocation. By default it uses operator new
class ASTFactory {
    ASTFactory() { }
public:
    // GetFactoryInstance ::= returns singleton instance of ASTFactory
    static ASTFactory *GetFactoryInstance();

    virtual ExpressionList* NewExpressionList();

    virtual Expression* NewNullLiteral(SourceLocation &loc);

    virtual Expression* NewUndefinedLiteral(SourceLocation &loc);

    virtual Expression* NewThisHolder(SourceLocation &loc);

    virtual Expression* NewIntegralLiteral(SourceLocation &loc, double value);

    virtual Expression* NewStringLiteral(SourceLocation &loc, std::string str);

    virtual Expression* NewArrayLiteral(SourceLocation &loc, ProxyArray arr);

    virtual Expression* NewObjectLiteral(SourceLocation &loc, ProxyObject obj);

    virtual Expression* NewIdentifier(SourceLocation &loc, std::string name);

    virtual Expression* NewBooleanLiteral(SourceLocation &loc, bool val);
    
    virtual Expression* NewArgumentList(SourceLocation &loc, ExpressionList *);
    
    virtual Expression* NewCallExpression(SourceLocation &loc, MemberAccessKind kind,
                                    Expression *func, Expression *args);

    virtual Expression *NewMemberExpression(SourceLocation &loc, MemberAccessKind kind, 
        Expression *expr, Expression *mem);

    virtual Expression *NewNewExpression(SourceLocation &loc,
                                    Expression *expr);
    
    virtual Expression *NewPrefixExpression(SourceLocation &loc,
                                    PrefixOperation op, Expression *expr);
    
    virtual Expression *NewPostfixExpression(SourceLocation &loc,
                                    PostfixOperation op, Expression *expr);
    
    virtual Expression *NewBinaryExpression(SourceLocation &loc,
                    BinaryOperation op, Expression *lhs, Expression *rhs);
    
    virtual Expression *NewAssignExpression(SourceLocation &loc,
                                    Expression *lhs, Expression *rhs);
    
    virtual Expression *NewTernaryExpression(SourceLocation &loc,
        Expression *first, Expression *second, Expression *third);
    
    virtual Expression *NewCommaExpression(SourceLocation &loc, ExpressionList *l);
    
    virtual Declaration *NewDeclaration(SourceLocation &loc, std::string name,
                                Expression *init = nullptr);
    
    virtual Expression *NewDeclarationList(SourceLocation &loc,
        std::vector<std::unique_ptr<Declaration>> decls);
    
    virtual Expression *NewDeclarationList(SourceLocation &loc);

    virtual Expression *NewBlockStatement(SourceLocation &loc,
                                            ExpressionList *list);
    
    virtual Expression *NewForStatement(SourceLocation &loc, ForKind kind,
            Expression *init, Expression *condition, Expression *update,
            Expression *body);
    
    virtual Expression *NewWhileStatement(SourceLocation &loc,
        Expression *condition, Expression *body);
    
    virtual Expression *NewDoWhileStatement(SourceLocation &loc,
        Expression *condition, Expression *body);
    
    virtual Expression *NewFunctionPrototype(SourceLocation &loc,
        std::string name, std::vector<std::string> args);
    
    virtual Expression *NewFunctionStatement(SourceLocation &loc,
        FunctionPrototype *proto, Expression *body);
    
    virtual Expression *NewIfStatement(SourceLocation &loc,
        Expression *condition, Expression *then);
    
    virtual Expression *NewIfElseStatement(SourceLocation &loc,
        Expression *condition, Expression *then, Expression *els);
    
    virtual Expression *NewReturnStatement(SourceLocation &loc,
        Expression *expr);
    
    virtual Expression *NewTryCatchStatement(SourceLocation &loc, Expression *tb,
        Expression *catch_expr, Expression *catch_block, Expression *finally);

    virtual Expression *NewBreakStatement(SourceLocation &loc,
            Expression *label = nullptr);

    virtual Expression *NewContinueStatement(SourceLocation &loc,
            Expression *label = nullptr);

    virtual Expression *NewLabelledStatement(SourceLocation &loc, std::string label,
            Expression *expr);

    virtual Expression *NewCaseClauseStatement(SourceLocation &loc, Expression *clause,
            Expression *stmt);

    virtual ClausesList *NewClausesList();

    virtual Expression *NewSwitchStatement(SourceLocation &loc, Expression *expr,
            ClausesList *clauses);

    virtual Expression *NewThrowStatement(SourceLocation &loc, Expression *expr);
};

}
}

#endif
