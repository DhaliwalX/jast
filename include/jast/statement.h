#ifndef STATEMENT_H_
#define STATEMENT_H_

#include "jast/expression.h"

#include <iostream>
namespace jast {

///  BlockStatment ::= class representing block statments
class BlockStatement : public Expression {
public:
    BlockStatement(Position &loc, ExpressionList *stmts)
        : Expression(loc), stmts_{ stmts }
    { }

    DEFINE_NODE_TYPE(BlockStatement);

    ExpressionList *statements() { return stmts_.get(); }
    void PushExpression(Expression *expr);
private:
    std::unique_ptr<ExpressionList> stmts_;
};

enum class ForKind {
    kForOf,
    kForIn
};

class ForStatement : public Expression {
public:
    ForStatement(Position &loc, ForKind kind, Expression* init,
        Expression* condition, Expression* update, Expression* body)
        : Expression(loc), kind_{ kind }, init_{ init }, condition_{ condition },
          update_{ update }, body_{ body }
    { }

    DEFINE_NODE_TYPE(ForStatement);

    Expression *init() { return init_.get(); }
    Expression *condition() { return condition_.get(); }
    Expression *update() { return update_.get(); }
    Expression *body() { return body_.get(); }

    ForKind kind() { return kind_; }

private:
    ForKind kind_;
    std::unique_ptr<Expression> init_;
    std::unique_ptr<Expression> condition_;
    std::unique_ptr<Expression> update_;
    std::unique_ptr<Expression> body_;
};

class WhileStatement : public Expression {
    using ExprPtr = Expression*; // just for convenience
public:
    WhileStatement(Position &loc, ExprPtr condition, ExprPtr body)
        : Expression(loc), condition_{ condition },
          body_{ body }
    { }

    DEFINE_NODE_TYPE(WhileStatement);

    ExprPtr condition() { return condition_.get(); }
    ExprPtr body() { return body_.get(); }

private:
    std::unique_ptr<Expression> condition_;
    std::unique_ptr<Expression> body_;
};

class BreakStatement : public Expression {
public:
    BreakStatement(Position &loc, Expression *label = nullptr)
        : Expression(loc), label_{ label }
    { }

    DEFINE_NODE_TYPE(BreakStatement);

    Expression *label() { return label_.get(); }
private:
    std::unique_ptr<Expression> label_;
};

class ContinueStatement : public Expression {
public:
    ContinueStatement(Position &loc, Expression *label = nullptr)
        : Expression(loc), label_{ label }
    { }

    DEFINE_NODE_TYPE(ContinueStatement);

    Expression *label() { return label_.get(); }

private:
    std::unique_ptr<Expression> label_;
};

class ThrowStatement : public Expression {
public:
    ThrowStatement(Position &loc, Expression *expr)
        : Expression(loc), expr_{expr}
    { }

    DEFINE_NODE_TYPE(ThrowStatement);

    Expression *expr() { return expr_.get(); }

private:
    std::unique_ptr<Expression> expr_;
};
  
class TryCatchStatement : public Expression {
public:
    TryCatchStatement(Position &loc, Expression *try_block,
            Expression *catch_expr, Expression *catch_block, Expression *finally)
        : Expression(loc), try_block_{ try_block }, catch_expr_{ catch_expr },
          catch_block_{ catch_block }, finally_{ finally }
    { }

    DEFINE_NODE_TYPE(TryCatchStatement);

    Expression *try_block() { return try_block_.get(); }
    Expression *catch_expr() { return catch_expr_.get(); }
    Expression *catch_block() { return catch_block_.get(); }
    Expression *finally() { return finally_.get(); }
private:
    std::unique_ptr<Expression> try_block_;
    std::unique_ptr<Expression> catch_expr_;
    std::unique_ptr<Expression> catch_block_;
    std::unique_ptr<Expression> finally_;
};

class LabelledStatement : public Expression {
public: 
    LabelledStatement(Position &loc, std::string &label, Expression *expr)
        : Expression(loc), label_{ label }, expr_{ expr }
    { }

    DEFINE_NODE_TYPE(LabelledStatement);

    Expression *expr() { return expr_.get(); }
    std::string &label() { return label_; }

private:
    std::string label_;
    std::unique_ptr<Expression> expr_;
};

class CaseClauseStatement : public Expression {
public:
    CaseClauseStatement(Position &loc,
        Expression *clause, Expression *stmt)
        : Expression(loc), clause_{ clause }, stmt_{ stmt }
    { }

    DEFINE_NODE_TYPE(CaseClauseStatement);

    Expression *clause() { return clause_.get(); }
    Expression *stmt() { return stmt_.get(); }

private:
    std::unique_ptr<Expression> clause_;
    std::unique_ptr<Expression> stmt_;
};

// helper class to store all cases under switch statement
class ClausesList {
public:
    using iterator = std::vector<CaseClauseStatement*>::iterator;

    ClausesList() : default_ { nullptr }, cases_{ }
    { }

    bool HasDefaultCase() { return def(); }

    bool SetDefaultCase(Expression *def)
    {
        if (HasDefaultCase())
            return false;

        default_.reset(def);
        return true;
    }

    void PushCase(CaseClauseStatement *stmt) { cases_.push_back(stmt); }

    ~ClausesList()
    {
        // ugly: need a better allocation and deallocation method
        for (auto &c : cases_)
            delete c;
    }

    std::vector<CaseClauseStatement*> *cases() { return &cases_; }
    Expression *def() { return default_.get(); }

    iterator begin() { return cases_.begin(); }
    iterator end() { return cases_.end(); }

    auto Size() { return cases_.size(); }
private:
    std::unique_ptr<Expression> default_;
    std::vector<CaseClauseStatement*> cases_;
};

class SwitchStatement : public Expression {
public:
    SwitchStatement(Position &loc, Expression *expr,
            ClausesList *clauses)
        : Expression(loc), expr_{ expr }, clauses_{ clauses }
    { }

    DEFINE_NODE_TYPE(SwitchStatement);
    bool HasDefaultCase() { return clauses_->HasDefaultCase(); }
    Expression *default_clause() { return clauses_->def(); }
    ClausesList *clauses() { return clauses_.get(); }
    Expression *expr() { return expr_.get(); }
private:
    std::unique_ptr<Expression> expr_;
    std::unique_ptr<ClausesList> clauses_;
};

class DoWhileStatement : public Expression {
    using ExprPtr = Expression*;
public:
    DoWhileStatement(Position &loc, ExprPtr condition, ExprPtr body)
        : Expression(loc), condition_{ condition },
          body_{ body }
    { }
    DEFINE_NODE_TYPE(DoWhileStatement);

    ExprPtr condition() { return condition_.get(); }
    ExprPtr body() { return body_.get(); }
private:
    std::unique_ptr<Expression> condition_;
    std::unique_ptr<Expression> body_;
};

// FunctionPrototype - captures the prototype of the function
// which includes the name of the function and 
class FunctionPrototype : public Expression {
public:
    FunctionPrototype(Position &loc, std::string name,
        std::vector<std::string> args)
        : Expression(loc), name_{ name }, args_{ std::move(args) }
    { }
    DEFINE_NODE_TYPE(FunctionPrototype);
    const std::string &GetName() const;
    const std::vector<std::string> &GetArgs() const;
private:
    std::string name_;
    std::vector<std::string> args_;
};

// FunctionStatement - captures the function statement
class FunctionStatement : public Expression {
public:
    FunctionStatement(Position &loc, 
        FunctionPrototype *proto, Expression *body)
        : Expression(loc), proto_{ (proto) }, body_{ body }
    { }

    DEFINE_NODE_TYPE(FunctionStatement);

    FunctionPrototype *proto() { return proto_.get(); }
    Expression *body() { return body_.get(); }
private:
    std::unique_ptr<FunctionPrototype> proto_;
    std::unique_ptr<Expression> body_;
};

class IfStatement : public Expression {
    using ExprPtr = Expression*;
public:
    IfStatement(Position &loc, ExprPtr cond, ExprPtr body)
        : Expression(loc), condition_{ (cond) }, body_{ (body) }
    { }

    DEFINE_NODE_TYPE(IfStatement);

    Expression *condition() { return condition_.get(); }
    Expression *body() { return body_.get(); }
private:
    std::unique_ptr<Expression> condition_;
    std::unique_ptr<Expression> body_;
};

class IfElseStatement : public Expression {
    using ExprPtr = Expression*;
public:
    IfElseStatement(Position &loc, ExprPtr cond, ExprPtr body, ExprPtr el)
    : Expression(loc), condition_{ (cond) },
      body_{ (body) },
      else_{ (el) }
    { }

    DEFINE_NODE_TYPE(IfElseStatement);

    Expression *condition() { return condition_.get(); }
    Expression *body() { return body_.get(); }
    Expression *els() { return else_.get(); }
private:
    std::unique_ptr<Expression> condition_;
    std::unique_ptr<Expression> body_;
    std::unique_ptr<Expression> else_;
};

class ReturnStatement : public Expression {
public:
    ReturnStatement(Position &loc, Expression *expr)
        : Expression(loc), expr_{ (expr) }
    { }

    Expression *expr() { return expr_.get(); }

    DEFINE_NODE_TYPE(ReturnStatement);
private:
    std::unique_ptr<Expression> expr_;
};

}

#endif
