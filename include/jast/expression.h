#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "jast/common.h"
#include "jast/source-locator.h"
#include "jast/scope.h"
#include "jast/handle.h"

#include <vector>
#include <string>
#include <map>
#include <cassert>

namespace jast {
#ifdef NO_EMIT_FUNCTION
#   define EMIT_FUNCTION
#else
#   define EMIT_FUNCTION
#endif

#define DEFINE_NODE_TYPE(Type) \
public: \
    friend class ASTVisitor; \
    friend class Expression; \
    friend class ASTBuilder; \
    friend class ASTFactory; \
    Type(const Position &pos, Scope *scope) \
        : Expression(pos, scope) \
    { } \
    virtual ~Type() = default; \
    bool Is##Type() const override { return true; }   \
    Handle<Type> As##Type() override \
    { return Handle<Type>(dynamic_cast<Type*>(this)); }  \
    ASTNodeType type() const override { return ASTNodeType::k##Type; }  \
    void Accept(ASTVisitor *visitor) override; \
protected: \
    static Handle<Type> Create(const Position &pos, Scope *scope) \
    { return MakeHandle<Type>(pos, scope); }

#define AST_NODE_LIST(M)    \
    M(NullLiteral)          \
    M(UndefinedLiteral)     \
    M(ThisHolder)           \
    M(IntegralLiteral)      \
    M(StringLiteral)        \
    M(TemplateLiteral)      \
    M(ArrayLiteral)         \
    M(ObjectLiteral)        \
    M(Identifier)           \
    M(BooleanLiteral)       \
    M(RegExpLiteral)       \
    M(ArgumentList)         \
    M(CallExpression)       \
    M(MemberExpression)     \
    M(NewExpression)        \
    M(PrefixExpression)     \
    M(PostfixExpression)    \
    M(BinaryExpression)     \
    M(AssignExpression)     \
    M(TernaryExpression)    \
    M(CommaExpression)      \
    M(Declaration)          \
    M(DeclarationList)      \
    M(IfStatement)          \
    M(IfElseStatement)      \
    M(ForStatement)         \
    M(WhileStatement)       \
    M(LabelledStatement)    \
    M(BreakStatement)       \
    M(ContinueStatement)    \
    M(SwitchStatement)      \
    M(CaseClauseStatement)  \
    M(TryCatchStatement)    \
    M(ThrowStatement)       \
    M(DoWhileStatement)     \
    M(BlockStatement)       \
    M(FunctionPrototype)    \
    M(FunctionStatement)    \
    M(ReturnStatement)

class ASTVisitor;

#define FORWARD_DECLARE(Type)   \
class Type;
AST_NODE_LIST(FORWARD_DECLARE)
#undef FORWARD_DECLARE

enum class ASTNodeType {
    kUnknownType,
#define AST_NODE_TYPE(type) k##type,
AST_NODE_LIST(AST_NODE_TYPE)
#undef AST_NODE_TYPE
    kNrType
};

extern const char *type_as_string[(int)ASTNodeType::kNrType];

class Expression : public RefCountObject {
protected:
    Expression(const Position &loc, Scope *scope) :
        loc_{ loc }, scope_{ scope }
    { }
public:
    virtual ~Expression() { }
    virtual void Accept(ASTVisitor *visitor) = 0;
    virtual bool ProduceRValue() { return true; }
    virtual ASTNodeType type() const = 0;
    virtual void SetScope(Scope *scope) { scope_ = scope; }
    virtual Scope *GetScope() { return scope_; }

// helper conversion functions
#define AS_EXPRESSION_FUNCTION(Type)    \
    virtual Handle<Type> As##Type() { assert(0 && "Expression is not " #Type); }
AST_NODE_LIST(AS_EXPRESSION_FUNCTION)
#undef AS_EXPRESSION_FUNCTION

    // helpful for constant folding
#define IS_EXPRESSION_FUNCTION(Type)    \
    virtual bool Is##Type() const { return false; }
AST_NODE_LIST(IS_EXPRESSION_FUNCTION)
#undef IS_EXPRESSION_FUNCTION

    const Position &loc() const { return loc_;}
private:
    Position loc_;
    Scope *scope_;
};

using ProxyArray = std::vector<Handle<Expression>>;
using ProxyObject = std::map<std::string, Handle<Expression>>;

// ExpressionList ::= helper class representing a list of expressions
class ExpressionList : public RefCountObject {
public:
    using iterator = std::vector<Handle<Expression>>::iterator;

    ExpressionList()
        : exprs_{ }
    { }

    void Insert(Handle<Expression> expr)
    {
        exprs_.push_back(expr);
    }

    size_t Size()
    {
        return exprs_.size();
    }

    std::vector<Handle<Expression>> &raw_list() { return exprs_; }

    ~ExpressionList()
    {
        exprs_.clear();
    }

    iterator begin() { return exprs_.begin(); }
    iterator end() { return exprs_.end(); }
private:
    std::vector<Handle<Expression>> exprs_;
};

class NullLiteral : public Expression {
public:
    NullLiteral(Position &loc, Scope *scope)
        : Expression(loc, scope)
    { }

    DEFINE_NODE_TYPE(NullLiteral);
};

class UndefinedLiteral : public Expression {
public:
    UndefinedLiteral(Position &loc, Scope *scope)
        : Expression(loc, scope)
    { }

    DEFINE_NODE_TYPE(UndefinedLiteral);
};

class ThisHolder : public Expression {
public:
    ThisHolder(Position &loc, Scope *scope)
        : Expression(loc, scope)
    { }

    bool ProduceRValue() override { return false; }
    DEFINE_NODE_TYPE(ThisHolder);
};

class IntegralLiteral : public Expression {
private:
    double value_;
public:
    IntegralLiteral(Position &loc, Scope *scope, double value)
        : Expression(loc, scope), value_(value)
    { }

    double value() { return value_; }
    DEFINE_NODE_TYPE(IntegralLiteral);
};

class StringLiteral : public Expression {
private:
    std::string str_;
public:
    StringLiteral(Position &loc, Scope *scope, const std::string &str)
        : Expression(loc, scope), str_(str)
    { }

    std::string &string() { return str_; }
    DEFINE_NODE_TYPE(StringLiteral);
};

class TemplateLiteral : public Expression {
private:
    std::string template_string_;

public:
    TemplateLiteral(Position &loc, Scope *scope, const std::string &template_string)
        : Expression(loc, scope), template_string_{ template_string }
    { }

    std::string &template_string() { return template_string_; }
    DEFINE_NODE_TYPE(TemplateLiteral);
};

class ArrayLiteral : public Expression {
public:
    ArrayLiteral(Position &loc, Scope *scope, ProxyArray exprs)
        : Expression(loc, scope), exprs_{ std::move(exprs) }
    { }

    ProxyArray &exprs() { return exprs_; }

    typename ProxyArray::size_type length() { return exprs_.size(); }

    DEFINE_NODE_TYPE(ArrayLiteral);
private:
    ProxyArray exprs_;
};

class ObjectLiteral : public Expression {
public:
    ObjectLiteral(Position &loc, Scope *scope, ProxyObject props)
        : Expression(loc, scope), Props{ std::move(props) }
    { }

    ProxyObject &proxy() { return Props; }
    bool IsEmpty() { return Props.empty(); }
    ProxyObject::size_type GetPropertyCount() { return Props.size(); }

    DEFINE_NODE_TYPE(ObjectLiteral);
private:
    ProxyObject Props;
};

class Identifier : public Expression {
private:
    std::string name_;
public:
    Identifier(Position &loc, Scope *scope, const std::string &name)
        : Expression(loc, scope), name_(name)
    { }

    const std::string &GetName() const { return name_; }
    bool ProduceRValue() override { return false; }
    DEFINE_NODE_TYPE(Identifier);
};

class BooleanLiteral : public Expression {
    bool pred_;
public:
    BooleanLiteral(Position &loc, Scope *scope, bool val)
        : Expression(loc, scope), pred_(val) { }

    bool pred() { return pred_; }
    DEFINE_NODE_TYPE(BooleanLiteral);
};

enum class RegExpFlags : uint {
    kGlobal,
    kUnicode,
    kIgnoreCase,
    kMultiline,
    kSticky
};

class RegExpLiteral : public Expression {
public:
    RegExpLiteral(Position &loc, Scope *scope, const std::string &regex,
            const std::vector<RegExpFlags> &flags)
        : Expression(loc, scope), regex_{regex}, flags_{ flags }
    { }


    std::string &regex() {
        return regex_;
    }

    std::vector<RegExpFlags> &flags() {
        return flags_;
    }
    DEFINE_NODE_TYPE(RegExpLiteral);
private:
    std::string regex_;
    std::vector<RegExpFlags> flags_;
};

class ArgumentList : public Expression {
public:
    ArgumentList(Position &loc, Scope *scope, Handle<ExpressionList> args)
        : Expression(loc, scope), args_{ std::move(args) }
    { }

    Handle<ExpressionList> args() { return args_; }
    auto length() { return args()->Size(); }

    DEFINE_NODE_TYPE(ArgumentList);
private:
    Handle<ExpressionList> args_;
};

enum class MemberAccessKind {
    kCall,
    kDot,
    kIndex,
    kNew    // to support expression like new function(a) { this.a = a; }(a);
};

// Example of member expression AST (a(g).b.e[3](f))
//                      kCall
//                      /    \          |
//                   kIndex    (f)
//                  /      \            |
//                kDot      [3]
//               /   \                  |
//            kCall   e
//           /     \                    |
//          a       (g)
class CallExpression : public Expression {
public:
    CallExpression(Position &loc, Scope *scope, MemberAccessKind kind,
        Handle<Expression> expr, Handle<Expression> member)
        : Expression(loc, scope), kind_{ kind }, expr_(expr), member_(member)
    { }

    Handle<Expression> member() { return member_; }
    MemberAccessKind kind() { return kind_; }

    Handle<Expression> expr() { return expr_; }
    bool ProduceRValue() override { return false; }
    DEFINE_NODE_TYPE(CallExpression);
private:
    MemberAccessKind kind_;
    Handle<Expression> expr_;
    Handle<Expression> member_;
};

// class DotMemberExpression : public Expression {
// public:
//     DotMemberExpression(Position &loc, Scope *scope, Handle<Expression> mem)
//         : Expression(loc, scope), mem_(mem)
//     { }
//     DEFINE_NODE_TYPE(DotMemberExpression);

//     Handle<Expression> member() { return mem_; }
//     bool ProduceRValue() override { return false; }
// private:
//     Handle<Expression> mem_;
// };

// class IndexMemberExpression : public Expression {
// public:
//     IndexMemberExpression(Position &loc, Scope *scope, Handle<Expression> expr)
//         : Expression(loc, scope), expr_{ expr }
//     { }

//     DEFINE_NODE_TYPE(IndexMemberExpression);
//     Handle<Expression> expr() { return expr_; }
//     bool ProduceRValue() override { return false; }
// private:
//     Handle<Expression> expr_;
// };

class MemberExpression : public Expression {
public:
    MemberExpression(Position &loc, Scope *scope, MemberAccessKind kind,
        Handle<Expression> expr, Handle<Expression> member)
        : Expression(loc, scope), kind_{ kind }, expr_(expr), member_(member)
    { }

    Handle<Expression> member() { return member_; }
    MemberAccessKind kind() { return kind_; }

    Handle<Expression> expr() { return expr_; }
    bool ProduceRValue() override { return false; }
    DEFINE_NODE_TYPE(MemberExpression);
private:
    MemberAccessKind kind_;
    Handle<Expression> expr_;
    Handle<Expression> member_;
};

class NewExpression : public Expression {
public:
    NewExpression(Position &loc, Scope *scope, Handle<Expression> member)
        : Expression(loc, scope), member_{ member }
    { }

    Handle<Expression> member() { return member_; }
    bool ProduceRValue() override { return false; }
    DEFINE_NODE_TYPE(NewExpression);
private:
    Handle<Expression> member_;
};

enum class PrefixOperation {
    kIncrement,
    kDecrement,
    kTypeOf,
    kDelete,
    kBitNot,
    kNot,
    kVoid
};

class PrefixExpression : public Expression {
public:

    PrefixExpression(Position &loc, Scope *scope, PrefixOperation op, Handle<Expression> expr)
        : Expression(loc, scope), op_{ op }, expr_{ expr }
    { }


    PrefixOperation op() { return op_; }
    Handle<Expression> expr() { return expr_; }
    DEFINE_NODE_TYPE(PrefixExpression);
private:
    PrefixOperation op_;
    Handle<Expression> expr_;
};

enum class PostfixOperation {
    kIncrement,
    kDecrement
};

class PostfixExpression : public Expression {
public:
    PostfixExpression(Position &loc, Scope *scope, PostfixOperation op, Handle<Expression> expr)
        : Expression(loc, scope), op_{ op }, expr_{ expr }
    { }


    PostfixOperation op() { return op_; }
    Handle<Expression> expr() { return expr_; }
    DEFINE_NODE_TYPE(PostfixExpression);
private:
    PostfixOperation op_;
    Handle<Expression> expr_;
};

enum class BinaryOperation {
    kAddition,
    kSubtraction,
    kMultiplication,
    kDivision,
    kMod,
    kShiftRight,
    kShiftLeft,
    kShiftZeroRight,
    kLessThan,
    kGreaterThan,
    kLessThanEqual,
    kGreaterThanEqual,
    kEqual,
    kNotEqual,
    kStrictEqual,
    kStrictNotEqual,
    kAnd,
    kOr,
    kBitAnd,
    kBitOr,
    kBitXor,
    kInstanceOf,
    kIn
};

class BinaryExpression : public Expression {
    DEFINE_NODE_TYPE(BinaryExpression);
public:

    BinaryOperation op() { return op_; }
    Handle<Expression> lhs() { return lhs_; }
    Handle<Expression> rhs() { return rhs_; }
private:
    BinaryOperation op_;
    Handle<Expression> lhs_;
    Handle<Expression> rhs_;
public:
    BinaryExpression(Position &loc, Scope *scope, BinaryOperation op,
        Handle<Expression> lhs, Handle<Expression> rhs)
        : Expression(loc, scope), op_(op), lhs_(lhs), rhs_(rhs) { }
};

class AssignExpression : public Expression {
public:
    AssignExpression(Position &loc, Scope *scope, Handle<Expression> lhs, Handle<Expression> rhs)
        : Expression(loc, scope), lhs_(lhs), rhs_(rhs) { }

    Handle<Expression> lhs() { return lhs_; }
    Handle<Expression> rhs() { return rhs_; }
    DEFINE_NODE_TYPE(AssignExpression);
private:
    Handle<Expression> lhs_;
    Handle<Expression> rhs_;
};

class TernaryExpression : public Expression {
public:
    TernaryExpression(Position &loc, Scope *scope, Handle<Expression> first,
                      Handle<Expression> second, Handle<Expression> third)
    : Expression(loc, scope), first_(first), second_(second),
        third_(third)
    { }

    Handle<Expression> first() { return first_; }
    Handle<Expression> second() { return second_; }
    Handle<Expression> third() { return third_; }
    DEFINE_NODE_TYPE(TernaryExpression);
private:
    Handle<Expression> first_;
    Handle<Expression> second_;
    Handle<Expression> third_;
};

class CommaExpression : public Expression {
public:
    CommaExpression(Position &loc, Scope *scope, Handle<ExpressionList> exprs)
        : Expression(loc, scope), exprs_{ exprs }
    { }

    Handle<ExpressionList> exprs() { return exprs_; }
    DEFINE_NODE_TYPE(CommaExpression);
private:
    Handle<ExpressionList> exprs_;
};

class Declaration : public Expression {
public:
    Declaration(Position &loc, Scope *scope, std::string name, Handle<Expression> init)
        : Expression(loc, scope), name_{ name }, init_{ init }
    { }


    std::string &name() { return name_; }

    Handle<Expression> expr() { return init_; }
    DEFINE_NODE_TYPE(Declaration);
private:
    std::string name_;
    Handle<Expression> init_;
};

class DeclarationList : public Expression {
public:
    DeclarationList(Position &loc, Scope *scope,
        std::vector<Handle<Declaration>> exprs)
        : Expression(loc, scope), exprs_{ std::move(exprs) }
    { }

    std::vector<Handle<Declaration>> &exprs() { return exprs_; }

    void Append(Declaration *decl)
    {
        exprs_.push_back(Handle<Declaration>(decl));
    }
    DEFINE_NODE_TYPE(DeclarationList);
private:
    std::vector<Handle<Declaration>> exprs_;
};

} // namespace jast

#endif  /* Expression.h */
