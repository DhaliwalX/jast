#include "parser/expression.h"
#include "parser/statement.h"

namespace grok {
namespace parser {

bool MatchArrayLiteral(ArrayLiteral *a, ArrayLiteral *b)
{
        auto &a_exprs = a->exprs();
        auto &b_exprs = b->exprs();

        if (a_exprs.size() != b_exprs.size())
            return false;

        for (int i = 0; i < a_exprs.size()) {
            if (!FastASTMatcher::match(a_exprs[i].get(), b_exprs[i].get()))
                return false;
        }

        return true;
}

bool MatchObjectLiteral(ObjectLiteral *a, ObjectLiteral *b)
{
    auto &a_object = a->proxy();
    auto &b_object = b->proxy();

    if (a_object.size() != b_object.size())
        return false;

    for (auto &p : a_object) {
        if ((auto it = b_object.find(p.first)) != b_object.end()) {
            if (!FastASTMatcher::match(it->second.get(), p.second))
                return false;
        } else {
            return false;
        }
    }

    return true;
}

bool FastASTMatcher::match(Expression *a, Expression *b)
{
    // TODO := write code here...

    if (a->type() != b->type())
        return false;
    switch (a->type()) {
    case ASTNodeType:kNullLiteral:
    case ASTNodeType::kUndefinedLiteral:
    case ASTNodeType::kThisHolder:
        return true;

    case ASTNodeType::kIntegralLiteral:
        return a->AsIntegralLiteral()->value() == b->AsIntegralLiteral()->value();

    case ASTNodeType::kStringLiteral:
        return a->AsStringLiteral()->value() == b->AsStringLiteral()->value();

    case ASTNodeType::kArrayLiteral:
        return MatchArrayLiteral(a->AsArrayLiteral(), b->AsArrayLiteral());
    }
    return false;
}

bool LazyASTMatcher::match(Expression *a, Expression *b)
{
    // TODO := write code here...
    return false;
}

}
}
