#ifndef ASTFACTORY_H_
#define ASTFACTORY_H_

#include "jast/expression.h"
#include "jast/statement.h"

namespace jast {

// ASTFactory ::= factory for all the AST nodes it can be override'd so as
// to provide custom allocation. By default it uses operator new
class ASTFactory {
    ASTFactory() { }
public:
    // GetFactoryInstance ::= returns singleton instance of ASTFactory
    static ASTFactory *GetFactoryInstance();

    virtual ExpressionList* NewExpressionList();

    virtual Expression* NewNullLiteral(Position &loc);

    virtual Expression* NewUndefinedLiteral(Position &loc);

    virtual Expression* NewThisHolder(Position &loc);

    virtual Expression* NewIntegralLiteral(Position &loc, double value);

    virtual Expression* NewStringLiteral(Position &loc, std::string str);

    virtual Expression* NewTemplateLiteral(Position &loc, std::string str);

    virtual Expression* NewArrayLiteral(Position &loc, ProxyArray arr);

    virtual Expression* NewObjectLiteral(Position &loc, ProxyObject obj);

    virtual Expression* NewIdentifier(Position &loc, std::string name);

    virtual Expression* NewBooleanLiteral(Position &loc, bool val);
    
    virtual Expression* NewArgumentList(Position &loc, ExpressionList *);
    
    virtual Expression* NewCallExpression(Position &loc, MemberAccessKind kind,
                                    Expression *func, Expression *args);

    virtual Expression *NewMemberExpression(Position &loc, MemberAccessKind kind, 
        Expression *expr, Expression *mem);

    virtual Expression *NewNewExpression(Position &loc,
                                    Expression *expr);
    
    virtual Expression *NewPrefixExpression(Position &loc,
                                    PrefixOperation op, Expression *expr);
    
    virtual Expression *NewPostfixExpression(Position &loc,
                                    PostfixOperation op, Expression *expr);
    
    virtual Expression *NewBinaryExpression(Position &loc,
                    BinaryOperation op, Expression *lhs, Expression *rhs);
    
    virtual Expression *NewAssignExpression(Position &loc,
                                    Expression *lhs, Expression *rhs);
    
    virtual Expression *NewTernaryExpression(Position &loc,
        Expression *first, Expression *second, Expression *third);
    
    virtual Expression *NewCommaExpression(Position &loc, ExpressionList *l);
    
    virtual Declaration *NewDeclaration(Position &loc, std::string name,
                                Expression *init = nullptr);
    
    virtual Expression *NewDeclarationList(Position &loc,
        std::vector<std::unique_ptr<Declaration>> decls);
    
    virtual Expression *NewDeclarationList(Position &loc);

    virtual Expression *NewBlockStatement(Position &loc,
                                            ExpressionList *list);
    
    virtual Expression *NewForStatement(Position &loc, ForKind kind,
            Expression *init, Expression *condition, Expression *update,
            Expression *body);
    
    virtual Expression *NewWhileStatement(Position &loc,
        Expression *condition, Expression *body);
    
    virtual Expression *NewDoWhileStatement(Position &loc,
        Expression *condition, Expression *body);
    
    virtual Expression *NewFunctionPrototype(Position &loc,
        std::string name, std::vector<std::string> args);
    
    virtual Expression *NewFunctionStatement(Position &loc,
        FunctionPrototype *proto, Expression *body);
    
    virtual Expression *NewIfStatement(Position &loc,
        Expression *condition, Expression *then);
    
    virtual Expression *NewIfElseStatement(Position &loc,
        Expression *condition, Expression *then, Expression *els);
    
    virtual Expression *NewReturnStatement(Position &loc,
        Expression *expr);
    
    virtual Expression *NewTryCatchStatement(Position &loc, Expression *tb,
        Expression *catch_expr, Expression *catch_block, Expression *finally);

    virtual Expression *NewBreakStatement(Position &loc,
            Expression *label = nullptr);

    virtual Expression *NewContinueStatement(Position &loc,
            Expression *label = nullptr);

    virtual Expression *NewLabelledStatement(Position &loc, std::string label,
            Expression *expr);

    virtual Expression *NewCaseClauseStatement(Position &loc, Expression *clause,
            Expression *stmt);

    virtual ClausesList *NewClausesList();

    virtual Expression *NewSwitchStatement(Position &loc, Expression *expr,
            ClausesList *clauses);

    virtual Expression *NewThrowStatement(Position &loc, Expression *expr);
};

}

#endif
