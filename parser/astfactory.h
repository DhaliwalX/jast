#ifndef ASTFACTORY_H_
#define ASTFACTORY_H_

#include "parser/expression.h"
#include "parser/statement.h"

namespace grok {
namespace parser {

// ASTFactory ::= factory for all the AST nodes
class ASTFactory final {
    ASTFactory() { }
public:
    // GetFactoryInstance ::= returns singleton instance of ASTFactory
    static ASTFactory *GetFactoryInstance();

    ExpressionList* NewExpressionList();

    Expression* NewNullLiteral(SourceLocation &loc);

    Expression* NewUndefinedLiteral(SourceLocation &loc);

    Expression* NewThisHolder(SourceLocation &loc);

    Expression* NewIntegralLiteral(SourceLocation &loc, double value);

    Expression* NewStringLiteral(SourceLocation &loc, std::string str);

    Expression* NewArrayLiteral(SourceLocation &loc, ProxyArray arr);

    Expression* NewObjectLiteral(SourceLocation &loc, ProxyObject obj);

    Expression* NewIdentifier(SourceLocation &loc, std::string name);

    Expression* NewBooleanLiteral(SourceLocation &loc, bool val);
    
    Expression* NewArgumentList(SourceLocation &loc, ExpressionList *);
    
    Expression* NewCallExpression(SourceLocation &loc, MemberAccessKind kind,
                                    Expression *func, Expression *args);

    Expression *NewMemberExpression(SourceLocation &loc, MemberAccessKind kind, 
        Expression *expr, Expression *mem);

    Expression *NewNewExpression(SourceLocation &loc,
                                    Expression *expr);
    
    Expression *NewPrefixExpression(SourceLocation &loc,
                                    PrefixOperation op, Expression *expr);
    
    Expression *NewPostfixExpression(SourceLocation &loc,
                                    PostfixOperation op, Expression *expr);
    
    Expression *NewBinaryExpression(SourceLocation &loc,
                    BinaryOperation op, Expression *lhs, Expression *rhs);
    
    Expression *NewAssignExpression(SourceLocation &loc,
                                    Expression *lhs, Expression *rhs);
    
    Expression *NewTernaryExpression(SourceLocation &loc,
        Expression *first, Expression *second, Expression *third);
    
    Expression *NewCommaExpression(SourceLocation &loc, ExpressionList *l);
    
    Declaration *NewDeclaration(SourceLocation &loc, std::string name,
                                Expression *init = nullptr);
    
    Expression *NewDeclarationList(SourceLocation &loc,
        std::vector<std::unique_ptr<Declaration>> decls);
    
    Expression *NewDeclarationList(SourceLocation &loc);

    Expression *NewBlockStatement(SourceLocation &loc,
                                            ExpressionList *list);
    
    Expression *NewForStatement(SourceLocation &loc, ForKind kind,
            Expression *init, Expression *condition, Expression *update,
            Expression *body);
    
    Expression *NewWhileStatement(SourceLocation &loc,
        Expression *condition, Expression *body);
    
    Expression *NewDoWhileStatement(SourceLocation &loc,
        Expression *condition, Expression *body);
    
    Expression *NewFunctionPrototype(SourceLocation &loc,
        std::string name, std::vector<std::string> args);
    
    Expression *NewFunctionStatement(SourceLocation &loc,
        FunctionPrototype *proto, Expression *body);
    
    Expression *NewIfStatement(SourceLocation &loc,
        Expression *condition, Expression *then);
    
    Expression *NewIfElseStatement(SourceLocation &loc,
        Expression *condition, Expression *then, Expression *els);
    
    Expression *NewReturnStatement(SourceLocation &loc,
        Expression *expr);
    
    Expression *NewTryCatchStatement(SourceLocation &loc, Expression *tb,
        Expression *catch_expr, Expression *catch_block, Expression *finally);

    Expression *NewBreakStatement(SourceLocation &loc,
            Expression *label = nullptr);

    Expression *NewContinueStatement(SourceLocation &loc,
            Expression *label = nullptr);

    Expression *NewLabelledStatement(SourceLocation &loc, std::string label,
            Expression *expr);

    Expression *NewCaseClauseStatement(SourceLocation &loc, Expression *clause,
            Expression *stmt);

    ClausesList *NewClausesList();

    Expression *NewSwitchStatement(SourceLocation &loc, Expression *expr,
            ClausesList *clauses);
};

}
}

#endif
