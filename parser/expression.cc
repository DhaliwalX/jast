#include "parser/expression.h"
#include "parser/astvisitor.h"

namespace grok { namespace parser {

#define DEFINE_ACCEPT(type) \
void type::Accept(ASTVisitor *v)    \
{   \
    v->Visit(this); \
}

AST_NODE_LIST(DEFINE_ACCEPT)
#undef AST_NODE_LIST

// std::ostream &NullLiteral::operator<<(std::ostream &os) const
// {
//     os << "(null)";
//     return os;
// }

// std::ostream &ThisHolder::operator<<(std::ostream &os) const
// {
//     return os << "(this)";
// }

// std::ostream &IntegralLiteral::operator<<(std::ostream &os) const
// {
//     os << "(" << value_ << ")";
//     return os;
// }

// std::ostream &StringLiteral::operator<<(std::ostream &os) const
// {
//     os << "('" << str_ << "')";
//     return os;
// }

// std::ostream &Identifier::operator<<(std::ostream &os) const
// {
//     os << "(" << name_ << ")";
//     return os;
// }

// std::ostream &BooleanLiteral::operator<<(std::ostream &os) const
// {
//     os << "(" << std::boolalpha << pred_ << ")";
//     return os;
// }

// std::ostream &PrefixExpression::operator<<(std::ostream &os) const 
// {
//     os << "(";
//     return *expr_ << os << ")";
// }

// std::ostream &PostfixExpression::operator<<(std::ostream &os) const 
// {
//     os << "(";
//     *expr_ << os << ")";
//     // if (tok_ == INC) 
//     //     os << "++";
//     // else if (tok_ == DEC)
//     //     os << "--";
//     return os;
// }

// std::ostream &BinaryExpression::operator<<(std::ostream &os) const
// {
//     os << "(";
//     // weird! but its C++ operator overloading (-;
//     *lhs_ << os << " ";
//     // os << token_type[op_] << " ";
//     *rhs_ << os << ")";

//     return os;
// }

// std::ostream &AssignExpression::operator<<(std::ostream &os) const
// {
//     os << "(";
//     *lhs_ << os << " ";
//     os << '=' << " ";
//     *rhs_ << os << ")";
//     return os;
// }

// std::ostream &TernaryExpression::operator<<(std::ostream &os) const
// {
//     os << "(";
//     *first_ << os << " ";
//     os << '?' << " ";
//     *second_ << os << " ";
//     os << ":" << " ";
//     *third_ << os << ")";

//     return os;
// }

// std::ostream &CommaExpression::operator<<(std::ostream &os) const
// {
//     os << "(";
//     for (auto expr : *exprs_) {
//         *expr << os << ", ";
//     }

//     return os << ")";
// }

// std::ostream &ArrayLiteral::operator<<(std::ostream &os) const
// {
//     os << "([ ";
//     for (auto &i : exprs_) {
//         *i << os << ", ";
//     }
//     os << " ]";
//     return os << ")";
// }

// std::ostream &ObjectLiteral::operator<<(std::ostream &os) const
// {
//     os << "({ ";
//     for (auto &Prop : Props) {
//         os << Prop.first << ": ";
//         *(Prop.second) << os << ",";
//     }
//     os << " })";
//     return os;
// }

// std::ostream &DotMemberExpression::operator<<(std::ostream &os) const
// {
//     return os << "((.) " << mem_->AsIdentifier()->GetName() << ")";
// }

// std::ostream &IndexMemberExpression::operator<<(std::ostream &os) const
// {
//     os << "([ ";
//     *expr_ << os << " ])";
//     return os;
// }

// std::ostream &MemberExpression::operator<<(std::ostream &os) const
// {
//     os << "(";
//     for (auto i : *members_) {
//         *i << os;
//     }
//     return os << ")";
// }

// std::ostream &CallExpression::operator<<(std::ostream &os) const
// {
//     os << "(";
//     *func_ << os;
//     for (auto i : *members_) {
//         *i << os;
//     }
//     return os << ")";
// }

// std::ostream &ArgumentList::operator<<(std::ostream &os) const
// {
//     os << "(";
//     for (auto &i : args_) {
//         *i << os << ",";
//     }
//     return os << ")";
// }

// std::ostream &NewExpression::operator<<(std::ostream &os) const
// {
//     os << "( new ";
//     return *member_ << os << ")";
// }

// std::ostream &Declaration::operator<<(std::ostream &os) const
// {
//     os << "( var " << name_;
//     if (init_) {
//         os << " = ";
//         *init_ << os;
//     }
//     return os << ")";
// }

// std::ostream &DeclarationList::operator<<(std::ostream &os) const
// {
//     os << "( ";
//     for (auto &expr : exprs_) {
//         *expr << os;
//         os << ", ";
//     }
//     return os << ")";
// }

} // namespace parser
} // namespace grok