The Syntax tree of JavaScript is stored using [`Expression`](./parser/expression.h).
`Expression` is base class for all the syntax tree nodes. `Expression` is a pure virtual class and is defined as

```C++
class Expression {
protected:
    Expression(SourceLocation &loc) :
        loc_{ loc }
    { }
public:
    virtual ~Expression() { }
    virtual void Accept(ASTVisitor *visitor) = 0;
    virtual bool ProduceRValue() { return true; }

// helper conversion functions
#define AS_EXPRESSION_FUNCTION(Type)    \
    virtual Type *As##Type() { return nullptr; }
AST_NODE_LIST(AS_EXPRESSION_FUNCTION)
#undef AS_EXPRESSION_FUNCTION

    // helpful for constant folding
#define IS_EXPRESSION_FUNCTION(Type)    \
    virtual bool Is##Type() { return false; }
AST_NODE_LIST(IS_EXPRESSION_FUNCTION)
#undef IS_EXPRESSION_FUNCTION

private:
    SourceLocation loc_;
};
```
Macro `AS_EXPRESSION_FUNCTION` is a helper macro which expands to casting functions
like

    virtual NullLiteral *AsNullLiteral() { return nullptr; }
    virtual UndefinedLiteral *AsUndefinedLiteral() { return nullptr; }
    ...

All these functions return `nullptr` because `Expression` itself is a pure virtual
class. Each class deriving from `Expression`, will override it's corresponding
`As{class}()` method. `IS_EXPRESSION_FUNCTION` is also same as `AS_EXPRESSION_FUNCTION`.

```C++
    virtual void Accept(ASTVisitor *v) = 0;
```
`Accept` function is a part of visitor-acceptor pattern. This is the main function
you will be dealing with. Every class deriving from `Expression` class overrides this
function by following definition

```C++
void type::Accept(ASTVisitor *v)
{
    v->Visit(this);
}
```

ASTVisitor is discussed in [astvisitor.md](./astvisitor.md).

Next class is `ExpressionList`. This class is useful for storing a list of `Expression`. Number of classes like `ArgumentList`, `CommaExpression` uses this list.
NOTE: `ExpressionList` owns each of `Expression` object. Therefore on deleting
ExpressionList will delete all the `Expression` object owned in the list.

Implementation

```C++
class ExpressionList {
public:
    using iterator = std::vector<Expression*>::iterator;

    ExpressionList()
        : exprs_{ }
    { }

    void Insert(Expression *expr)
    {
        exprs_.push_back(expr);
    }

    size_t Size()
    {
        return exprs_.size();
    }

    // returns the inner container of Expressions
    std::vector<Expression *> *raw_list() { return &exprs_; }

    ~ExpressionList()
    {
        for (auto &expr : exprs_) {
            delete expr;
        }
        exprs_.clear();
    }

    iterator begin() { return exprs_.begin(); }
    iterator end() { return exprs_.end(); }
private:
    std::vector<Expression *> exprs_;
};
```


