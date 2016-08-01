#include "parser/statement.h"

using namespace grok::parser;

// std::ostream &BlockStatement::operator<<(std::ostream &os) const
// {
//     os << "({";
//     for (auto &stmt : stmts_) {
//         *stmt << os;
//     }
//     return os << "})";
// }

void BlockStatement::PushExpression(Expression *expr)
{
    stmts_->Insert(expr);
}

// std::ostream &ForStatement::operator<<(std::ostream &os) const
// {
//     os << "(for (";
//     *init_ << os;
//     os << ";";
//     *condition_ << os;
//     os << ";";
//     *update_ << os;
//     os << ") {";
//     *body_ << os;
//     os << "})";
//     return os;
// }

// std::ostream &WhileStatement::operator<<(std::ostream &os) const
// {
//     os << "(while (";
//     *condition_ << os;
//     os << ") {";
//     *body_ << os;
//     os << "})";
//     return os;
// }

// std::ostream &SwitchStatement::operator<<(std::ostream &os) const
// {
//     os << "(switch (";
//     *expr_ << os;
//     os << ") {";
    
//     ClausesList::iterator i = clauses()->begin();
//     ClausesList::iterator e = clauses()->end();

//     while (i != e) {
//         *i << os;
//     }
//     return os << ")}";
// }

// std::ostream &ContinueStatement::operator<<(std::ostream &os) const
// {
//     os << "(continue ";
//     return label_ ? (*label_ << os << ")") : (os << ")");
// }

// std::ostream &BreakStatement::operator<<(std::ostream &os) const
// {
//     os << "(break ";
//     return label() ? (*label() << os << ")") : (os << ")");
// }

// std::ostream &TryCatchStatement::operator<<(std::osteram &os) const
// {
//     os << "(try {";
//     *try_block() << os << "} catch (";
//     *catch_expr() << os << ") { ";
//     *catch_block() << os << "} finally {";
//     return *finally() << os << "})";
// }

// std::ostream &LabelledStatement::operator<<(std::ostream &os) const
// {
//     return os << "(label " << label() << ")";
// }

// std::ostream &CaseClauseStatement::operator<<(std::ostream &os) const
// {
//     os << "(case ";
//     *clause() << os << ": {";
//     *stmt() << os << " }";
//     return os << ")";
// }

// std::ostream &DoWhileStatement::operator<<(std::ostream &os) const
// {
//     os << "(do {";
//     *body_ << os;
//     os << "}) while (";
//     *condition_ << os;
//     return os << ")";
// }

// std::ostream &FunctionPrototype::operator<<(std::ostream &os) const
// {
//     os << "function " << name_ << "(";

//     for (auto &i : args_) {
//         os << i << ", ";
//     }
//     os << ")";

//     return os;
// }

const std::string &FunctionPrototype::GetName() const
{
    return name_;
}

const std::vector<std::string> &FunctionPrototype::GetArgs() const
{
    return args_;
}

// std::ostream &FunctionStatement::operator<<(std::ostream &os) const
// {
//     os << "(";
//     *proto_ << os << " {";
//     *body_ << os;
//     return os << "})";
// }

// std::ostream &IfStatement::operator<<(std::ostream &os) const
// {
//     os << "(if (";
//     *condition_ << os << ") {";
//     *body_ << os;
//     os << "})";
//     return os;
// }

// std::ostream &IfElseStatement::operator<<(std::ostream &os) const
// {
//     os << "(if (";
//     *condition_ << os << ") {";
//     *body_ << os;
//     os << "} else {";
//     *else_ << os;
//     os << "})";
//     return os;
// }
