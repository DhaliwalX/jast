#ifndef AST_VISITOR_H_
#define AST_VISITOR_H_

#include "parser/expression.h"
#include "parser/statement.h"

namespace grok {
namespace parser {

class ASTVisitor {
public:
    ASTVisitor() = default;
    virtual ~ASTVisitor() = default;

#define DECLARE_VISITOR_METHOD(type) virtual void Visit(type *) = 0;
AST_NODE_LIST(DECLARE_VISITOR_METHOD)
#undef DECLARE_VISITOR_METHOD
};

}
}


#endif
