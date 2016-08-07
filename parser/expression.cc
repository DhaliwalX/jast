#include "parser/expression.h"
#include "parser/astvisitor.h"

namespace grok { namespace parser {

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

} // namespace parser
} // namespace grok