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

    virtual Handle<ExpressionList> NewExpressionList();

    virtual Handle<Expression> NewNullLiteral(Position &loc, Scope *scope);

    virtual Handle<Expression> NewUndefinedLiteral(Position &loc, Scope *scope);

    virtual Handle<Expression> NewThisHolder(Position &loc, Scope *scope);

    virtual Handle<Expression> NewIntegralLiteral(Position &loc, Scope *scope, double value);

    virtual Handle<Expression> NewStringLiteral(Position &loc, Scope *scope, std::string str);

    virtual Handle<Expression> NewRegExpLiteral(Position &loc, Scope *scope, std::string str,
        const std::vector<RegExpFlags> &flags);

    virtual Handle<Expression> NewTemplateLiteral(Position &loc, Scope *scope, std::string str);

    virtual Handle<Expression> NewArrayLiteral(Position &loc, Scope *scope, ProxyArray arr);

    virtual Handle<Expression> NewObjectLiteral(Position &loc, Scope *scope, ProxyObject obj);

    virtual Handle<Expression> NewIdentifier(Position &loc, Scope *scope, std::string name);

    virtual Handle<Expression> NewBooleanLiteral(Position &loc, Scope *scope, bool val);
    
    virtual Handle<Expression> NewArgumentList(Position &loc, Scope *scope, Handle<ExpressionList>);
    
    virtual Handle<Expression> NewCallExpression(Position &loc, Scope *scope, MemberAccessKind kind,
                                    Handle<Expression> func, Handle<Expression> args);

    virtual Handle<Expression> NewMemberExpression(Position &loc, Scope *scope, MemberAccessKind kind, 
        Handle<Expression> expr, Handle<Expression> mem);

    virtual Handle<Expression> NewNewExpression(Position &loc, Scope *scope,
                                    Handle<Expression> expr);
    
    virtual Handle<Expression> NewPrefixExpression(Position &loc, Scope *scope,
                                    PrefixOperation op, Handle<Expression> expr);
    
    virtual Handle<Expression> NewPostfixExpression(Position &loc, Scope *scope,
                                    PostfixOperation op, Handle<Expression> expr);
    
    virtual Handle<Expression> NewBinaryExpression(Position &loc, Scope *scope,
                    BinaryOperation op, Handle<Expression> lhs, Handle<Expression> rhs);
    
    virtual Handle<Expression> NewAssignExpression(Position &loc, Scope *scope,
                                    Handle<Expression> lhs, Handle<Expression> rhs);
    
    virtual Handle<Expression> NewTernaryExpression(Position &loc, Scope *scope,
        Handle<Expression> first, Handle<Expression> second, Handle<Expression> third);
    
    virtual Handle<Expression> NewCommaExpression(Position &loc, Scope *scope, Handle<ExpressionList> l);
    
    virtual Handle<Declaration> NewDeclaration(Position &loc, Scope *scope, std::string name,
                                Handle<Expression> init = nullptr);
    
    virtual Handle<Expression> NewDeclarationList(Position &loc, Scope *scope,
        std::vector<Handle<Declaration>> decls);
    
    virtual Handle<Expression> NewDeclarationList(Position &loc, Scope *scope);

    virtual Handle<Expression> NewBlockStatement(Position &loc, Scope *scope,
                                            Handle<ExpressionList> list);
    
    virtual Handle<Expression> NewForStatement(Position &loc, Scope *scope, ForKind kind,
            Handle<Expression> init, Handle<Expression> condition, Handle<Expression> update,
            Handle<Expression> body);
    
    virtual Handle<Expression> NewWhileStatement(Position &loc, Scope *scope,
        Handle<Expression> condition, Handle<Expression> body);
    
    virtual Handle<Expression> NewDoWhileStatement(Position &loc, Scope *scope,
        Handle<Expression> condition, Handle<Expression> body);
    
    virtual Handle<Expression> NewFunctionPrototype(Position &loc, Scope *scope,
        std::string name, std::vector<std::string> args);
    
    virtual Handle<Expression> NewFunctionStatement(Position &loc, Scope *scope,
        Handle<FunctionPrototype> proto, Handle<Expression> body);
    
    virtual Handle<Expression> NewIfStatement(Position &loc, Scope *scope,
        Handle<Expression> condition, Handle<Expression> then);
    
    virtual Handle<Expression> NewIfElseStatement(Position &loc, Scope *scope,
        Handle<Expression> condition, Handle<Expression> then, Handle<Expression> els);
    
    virtual Handle<Expression> NewReturnStatement(Position &loc, Scope *scope,
        Handle<Expression> expr);
    
    virtual Handle<Expression> NewTryCatchStatement(Position &loc, Scope *scope, Handle<Expression> tb,
        Handle<Expression> catch_expr, Handle<Expression> catch_block, Handle<Expression> finally);

    virtual Handle<Expression> NewBreakStatement(Position &loc, Scope *scope,
            Handle<Expression> label = nullptr);

    virtual Handle<Expression> NewContinueStatement(Position &loc, Scope *scope,
            Handle<Expression> label = nullptr);

    virtual Handle<Expression> NewLabelledStatement(Position &loc, Scope *scope, std::string label,
            Handle<Expression> expr);

    virtual Handle<Expression> NewCaseClauseStatement(Position &loc, Scope *scope, Handle<Expression> clause,
            Handle<Expression> stmt);

    virtual Handle<ClausesList> NewClausesList();

    virtual Handle<Expression> NewSwitchStatement(Position &loc, Scope *scope, Handle<Expression> expr,
            Handle<ClausesList> clauses);

    virtual Handle<Expression> NewThrowStatement(Position &loc, Scope *scope, Handle<Expression> expr);
};

}

#endif
