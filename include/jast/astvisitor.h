#ifndef AST_VISITOR_H_
#define AST_VISITOR_H_

#include "jast/expression.h"
#include "jast/statement.h"

namespace jast {

// More strict visitor which wants you to override every visit member
class BasicASTVisitor {
public:
    BasicASTVisitor() = default;
    virtual ~BasicASTVisitor() = default;

#define DECLARE_VISITOR_METHOD(type) virtual void Visit(type *) = 0;
AST_NODE_LIST(DECLARE_VISITOR_METHOD)
#undef DECLARE_VISITOR_METHOD
};


// Lesser strict version of above visitor
class ASTVisitor : public BasicASTVisitor {
public:
#define DECLARE_VISITOR_METHOD(type) void Visit(type *) override {}
AST_NODE_LIST(DECLARE_VISITOR_METHOD)
#undef DECLARE_VISITOR_METHOD
};

}

#endif
