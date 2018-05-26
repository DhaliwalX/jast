#ifndef STATEMENT_H_
#define STATEMENT_H_

#include "jast/expression.h"

#include <iostream>
namespace jast {

///  BlockStatment ::= class representing block statments
class BlockStatement : public Expression {
public:
    BlockStatement(Position &loc, Scope *scope, Handle<ExpressionList> stmts)
        : Expression(loc, scope), stmts_{ stmts }
    { }


    Handle<ExpressionList> statements() { return stmts_; }
    void PushExpression(Handle<Expression> expr);
    DEFINE_NODE_TYPE(BlockStatement);
private:
    Handle<ExpressionList> stmts_;
};

enum class ForKind {
    kForOf,
    kForIn
};

class ForStatement : public Expression {
public:
    ForStatement(Position &loc, Scope *scope, ForKind kind, Handle<Expression> init,
        Handle<Expression> condition, Handle<Expression> update, Handle<Expression> body)
        : Expression(loc, scope), kind_{ kind }, init_{ init }, condition_{ condition },
          update_{ update }, body_{ body }
    { }


    Handle<Expression> init() { return init_; }
    Handle<Expression> condition() { return condition_; }
    Handle<Expression> update() { return update_; }
    Handle<Expression> body() { return body_; }

    ForKind kind() { return kind_; }

    DEFINE_NODE_TYPE(ForStatement);
private:
    ForKind kind_;
    Handle<Expression> init_;
    Handle<Expression> condition_;
    Handle<Expression> update_;
    Handle<Expression> body_;
};

class WhileStatement : public Expression {
    using ExprPtr = Handle<Expression>; // just for convenience
public:
    WhileStatement(Position &loc, Scope *scope, ExprPtr condition, ExprPtr body)
        : Expression(loc, scope), condition_{ condition },
          body_{ body }
    { }


    ExprPtr condition() { return condition_; }
    ExprPtr body() { return body_; }

    DEFINE_NODE_TYPE(WhileStatement);
private:
    Handle<Expression> condition_;
    Handle<Expression> body_;
};

class BreakStatement : public Expression {
public:
    BreakStatement(Position &loc, Scope *scope, Handle<Expression> label = nullptr)
        : Expression(loc, scope), label_{ label }
    { }


    Handle<Expression> label() { return label_; }
    DEFINE_NODE_TYPE(BreakStatement);
private:
    Handle<Expression> label_;
};

class ContinueStatement : public Expression {
public:
    ContinueStatement(Position &loc, Scope *scope, Handle<Expression> label = nullptr)
        : Expression(loc, scope), label_{ label }
    { }


    Handle<Expression> label() { return label_; }

    DEFINE_NODE_TYPE(ContinueStatement);
private:
    Handle<Expression> label_;
};

class ThrowStatement : public Expression {
public:
    ThrowStatement(Position &loc, Scope *scope, Handle<Expression> expr)
        : Expression(loc, scope), expr_{expr}
    { }


    Handle<Expression> expr() { return expr_; }

    DEFINE_NODE_TYPE(ThrowStatement);
private:
    Handle<Expression> expr_;
};
  
class TryCatchStatement : public Expression {
public:
    TryCatchStatement(Position &loc, Scope *scope, Handle<Expression> try_block,
            Handle<Expression> catch_expr, Handle<Expression> catch_block, Handle<Expression> finally)
        : Expression(loc, scope), try_block_{ try_block }, catch_expr_{ catch_expr },
          catch_block_{ catch_block }, finally_{ finally }
    { }


    Handle<Expression> try_block() { return try_block_; }
    Handle<Expression> catch_expr() { return catch_expr_; }
    Handle<Expression> catch_block() { return catch_block_; }
    Handle<Expression> finally() { return finally_; }
    DEFINE_NODE_TYPE(TryCatchStatement);
private:
    Handle<Expression> try_block_;
    Handle<Expression> catch_expr_;
    Handle<Expression> catch_block_;
    Handle<Expression> finally_;
};

class LabelledStatement : public Expression {
public: 
    LabelledStatement(Position &loc, Scope *scope, std::string &label, Handle<Expression> expr)
        : Expression(loc, scope), label_{ label }, expr_{ expr }
    { }


    Handle<Expression> expr() { return expr_; }
    std::string &label() { return label_; }

    DEFINE_NODE_TYPE(LabelledStatement);
private:
    std::string label_;
    Handle<Expression> expr_;
};

class CaseClauseStatement : public Expression {
public:
    CaseClauseStatement(Position &loc, Scope *scope,
        Handle<Expression> clause, Handle<Expression> stmt)
        : Expression(loc, scope), clause_{ clause }, stmt_{ stmt }
    { }


    Handle<Expression> clause() { return clause_; }
    Handle<Expression> stmt() { return stmt_; }

    DEFINE_NODE_TYPE(CaseClauseStatement);
private:
    Handle<Expression> clause_;
    Handle<Expression> stmt_;
};

// helper class to store all cases under switch statement
class ClausesList {
public:
    using iterator = std::vector<Handle<CaseClauseStatement>>::iterator;

    ClausesList() : default_ { nullptr }, cases_{ }
    { }

    bool HasDefaultCase() { return !!def(); }

    bool SetDefaultCase(Handle<Expression> def)
    {
        if (HasDefaultCase())
            return false;

        default_ = def;
        return true;
    }

    void PushCase(Handle<CaseClauseStatement> stmt) { cases_.push_back(stmt); }

    virtual ~ClausesList()
    {
    }

    std::vector<Handle<CaseClauseStatement>> *cases() { return &cases_; }
    Handle<Expression> def() { return default_; }

    iterator begin() { return cases_.begin(); }
    iterator end() { return cases_.end(); }

    auto Size() { return cases_.size(); }
private:
    Handle<Expression> default_;
    std::vector<Handle<CaseClauseStatement>> cases_;
};

class SwitchStatement : public Expression {
public:
    SwitchStatement(Position &loc, Scope *scope, Handle<Expression> expr,
            Handle<ClausesList> clauses)
        : Expression(loc, scope), expr_{ expr }, clauses_{ clauses }
    { }

    bool HasDefaultCase() { return clauses_->HasDefaultCase(); }
    Handle<Expression> default_clause() { return clauses_->def(); }
    Handle<ClausesList> clauses() { return clauses_; }
    Handle<Expression> expr() { return expr_; }
    DEFINE_NODE_TYPE(SwitchStatement);
private:
    Handle<Expression> expr_;
    Handle<ClausesList> clauses_;
};

class DoWhileStatement : public Expression {
    using ExprPtr = Handle<Expression>;
    DEFINE_NODE_TYPE(DoWhileStatement);
public:
    DoWhileStatement(Position &loc, Scope *scope, ExprPtr condition, ExprPtr body)
        : Expression(loc, scope), condition_{ condition },
          body_{ body }
    { }

    ExprPtr condition() { return condition_; }
    ExprPtr body() { return body_; }
private:
    Handle<Expression> condition_;
    Handle<Expression> body_;
};

// FunctionPrototype - captures the prototype of the function
// which includes the name of the function and 
class FunctionPrototype : public Expression {
    DEFINE_NODE_TYPE(FunctionPrototype);
public:
    FunctionPrototype(Position &loc, Scope *scope, std::string name,
        std::vector<std::string> args)
        : Expression(loc, scope), name_{ name }, args_{ std::move(args) }
    { }
    const std::string &GetName() const;
    const std::vector<std::string> &GetArgs() const;
private:
    std::string name_;
    std::vector<std::string> args_;
};

// FunctionStatement - captures the function statement
class FunctionStatement : public Expression {
    DEFINE_NODE_TYPE(FunctionStatement);
public:
    FunctionStatement(Position &loc, Scope *scope, 
        Handle<FunctionPrototype> proto, Handle<Expression> body)
        : Expression(loc, scope), proto_{ (proto) }, body_{ body }
    { }


    Handle<FunctionPrototype> proto() { return proto_; }
    Handle<Expression> body() { return body_; }
private:
    Handle<FunctionPrototype> proto_;
    Handle<Expression> body_;
};

class IfStatement : public Expression {
    DEFINE_NODE_TYPE(IfStatement);
    using ExprPtr = Handle<Expression>;
public:
    IfStatement(Position &loc, Scope *scope, ExprPtr cond, ExprPtr body)
        : Expression(loc, scope), condition_{ (cond) }, body_{ (body) }
    { }


    Handle<Expression> condition() { return condition_; }
    Handle<Expression> body() { return body_; }
private:
    Handle<Expression> condition_;
    Handle<Expression> body_;
};

class IfElseStatement : public Expression {
    DEFINE_NODE_TYPE(IfElseStatement);
    using ExprPtr = Handle<Expression>;
public:
    IfElseStatement(Position &loc, Scope *scope, ExprPtr cond, ExprPtr body, ExprPtr el)
    : Expression(loc, scope), condition_{ (cond) },
      body_{ (body) },
      else_{ (el) }
    { }


    Handle<Expression> condition() { return condition_; }
    Handle<Expression> body() { return body_; }
    Handle<Expression> els() { return else_; }
private:
    Handle<Expression> condition_;
    Handle<Expression> body_;
    Handle<Expression> else_;
};

class ReturnStatement : public Expression {
public:
    ReturnStatement(Position &loc, Scope *scope, Handle<Expression> expr)
        : Expression(loc, scope), expr_{ (expr) }
    { }

    Handle<Expression> expr() { return expr_; }

    DEFINE_NODE_TYPE(ReturnStatement);
private:
    Handle<Expression> expr_;
};

}

#endif
