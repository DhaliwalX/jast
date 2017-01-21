#include "jast/expression.h"
#include "jast/astvisitor.h"

namespace jast {

#define DEFINE_ACCEPT(type) \
void type::Accept(ASTVisitor *v)    \
{   \
    v->Visit(this); \
}

AST_NODE_LIST(DEFINE_ACCEPT)
#undef DEFINE_ACCEPT

const char *type_as_string[(int)ASTNodeType::kNrType] = {
    "kUnknownType",
#define AS_STRING(type) #type,
AST_NODE_LIST(AS_STRING)
#undef AS_STRING
};

} // namespace jast