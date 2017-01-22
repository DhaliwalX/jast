#include "pretty-printer.h"

namespace printer {

using namespace jast;

void PrettyPrinter::Visit(NullLiteral *literal)
{
    os() << "null";
}

void PrettyPrinter::Visit(UndefinedLiteral *literal)
{
    os() << "undefined";
}

void PrettyPrinter::Visit(ThisHolder *holder)
{
    os() << "this";
}

void PrettyPrinter::Visit(IntegralLiteral *literal)
{
    os() << literal->value();
}

void PrettyPrinter::Visit(StringLiteral *literal)
{
    os() << "'" << literal->string() << "'";
}

void PrettyPrinter::Visit(TemplateLiteral *literal)
{
    os() << "`" << literal->template_string() << "`";
}

void PrettyPrinter::Visit(ArrayLiteral *literal)
{
    ProxyArray &arr = literal->exprs();

    os() << "[ ";
    for (auto &expr : arr) {
        expr->Accept(this);
        os() << ", ";
    }
    os() << "]";
}

void PrettyPrinter::Visit(ObjectLiteral *literal)
{
    ProxyObject &obj = literal->proxy();

    os() << "{ ";
    for (auto &p : obj) {
        os() << '\'' << p.first << "': ";
        p.second->Accept(this);
        os() << ", ";
    }
    os() << "}";
}

void PrettyPrinter::Visit(Identifier *id)
{
    os() << id->GetName();
}

void PrettyPrinter::Visit(BooleanLiteral *literal)
{
    os() << std::boolalpha << literal->pred();
}

void PrettyPrinter::Visit(RegExpLiteral *reg)
{
    os() << '/' << '/';
}

void PrettyPrinter::Visit(ArgumentList *args)
{
    auto list = args->args();
    os() << "(";
    if (!list) {
        os() << ")";
        return;
    }

    auto b = list->begin();
    auto e = list->end() - 1;

    for (; b != e; ++b) {
        (*b)->Accept(this);
        os() << ", ";
    }
    (*e)->Accept(this);
    os() << ")";
}

void PrettyPrinter::Visit(CallExpression *expr)
{
    expr->expr()->Accept(this);

    switch (expr->kind()) {
    case MemberAccessKind::kCall:
        expr->member()->Accept(this);
        break;
    case MemberAccessKind::kDot:
        os() << ".";
        expr->member()->Accept(this);
        break;
    case MemberAccessKind::kIndex:
        os() << "[";
        expr->member()->Accept(this);
        os() << "]";
        break;
    default:
        throw std::runtime_error("");
    }
}

void PrettyPrinter::Visit(MemberExpression *expr)
{
    expr->expr()->Accept(this);

    switch (expr->kind()) {
    case MemberAccessKind::kCall:
        expr->member()->Accept(this);
        break;
    case MemberAccessKind::kDot:
        os() << ".";
        expr->member()->Accept(this);
        break;
    case MemberAccessKind::kIndex:
        os() << "[";
        expr->member()->Accept(this);
        os() << "]";
        break;
    default:
        throw std::runtime_error("");
    }    
}

void PrettyPrinter::Visit(NewExpression *expr)
{
    os() << "new ";
    expr->member()->Accept(this);
}

void PrettyPrinter::Visit(PrefixExpression *expr)
{
    switch (expr->op()) {
    case PrefixOperation::kIncrement:
        os() << "++";
        break;
    case PrefixOperation::kDecrement:
        os() << "--";
        break;
    case PrefixOperation::kTypeOf:
        os() << "typeof ";
        break;
    case PrefixOperation::kDelete:
        os() << "delete ";
        break;
    case PrefixOperation::kBitNot:
        os() << "~";
        break;
    case PrefixOperation::kNot:
        os() << "!";
        break;
    case PrefixOperation::kVoid:
        os() << "void ";
        break;
    default:
        throw std::runtime_error("invalid prefix operator");
    }
    expr->expr()->Accept(this);
}

void PrettyPrinter::Visit(PostfixExpression *expr)
{
    expr->expr()->Accept(this);
    switch (expr->op()) {
    case PostfixOperation::kIncrement:
        os() << "++";
        break;
    case PostfixOperation::kDecrement:
        os() << "--";
        break;
    default:
        throw std::runtime_error("invalid postfix operator");
    }
}

void PrettyPrinter::Visit(BinaryExpression *expr)
{
    expr->lhs()->Accept(this);
    os() << " ";
    switch (expr->op()) {
    case BinaryOperation::kAddition:
        os() << "+";
        break;
    case BinaryOperation::kMultiplication:
        os() << "*";
        break;
    case BinaryOperation::kSubtraction:
        os() << "-";
        break;
    case BinaryOperation::kDivision:
        os() << "/";
        break;
    case BinaryOperation::kMod:
        os() << "%";
        break;
    case BinaryOperation::kShiftRight:
        os() << ">>";
        break;
    case BinaryOperation::kShiftZeroRight:
        os() << ">>>";
        break;
    case BinaryOperation::kShiftLeft:
        os() << "<<";
        break;
    case BinaryOperation::kLessThan:
        os() << "<";
        break;
    case BinaryOperation::kGreaterThan:
        os() << ">";
        break;
    case BinaryOperation::kLessThanEqual:
        os() << "<=";
        break;
    case BinaryOperation::kGreaterThanEqual:
        os() << ">=";
        break;
    case BinaryOperation::kEqual:
        os() << "==";
        break;
    case BinaryOperation::kNotEqual:
        os() << "!=";
        break;
    case BinaryOperation::kStrictEqual:
        os() << "===";
        break;
    case BinaryOperation::kStrictNotEqual:
        os() << "!==";
        break;
    case BinaryOperation::kAnd:
        os() << "&&";
        break;
    case BinaryOperation::kOr:
        os() << "||";
        break;
    case BinaryOperation::kBitAnd:
        os() << "&";
        break;
    case BinaryOperation::kBitOr:
        os() << "|";
        break;
    case BinaryOperation::kBitXor:
        os() << "^";
        break;
    case BinaryOperation::kInstanceOf:
        os() << "instanceof";
        break;
    default: throw std::runtime_error("invalid binary operation");
    }
    os() << " ";
    expr->rhs()->Accept(this);
}

void PrettyPrinter::Visit(AssignExpression *expr)
{
    expr->lhs()->Accept(this);
    os() << " = ";
    expr->rhs()->Accept(this);
}

void PrettyPrinter::Visit(TernaryExpression *expr)
{
    expr->first()->Accept(this);
    os() << " ? ";
    expr->second()->Accept(this);
    os() << " : ";
    expr->third()->Accept(this);
}

void PrettyPrinter::Visit(CommaExpression *expr)
{
    auto exprs = expr->exprs();
    if (!exprs->Size())
        return;
    auto b = exprs->begin();
    auto e = exprs->end() - 1;

    for (; b != e; ++b) {
        (*b)->Accept(this);
        os() << ", ";
    }

    (*e)->Accept(this);
}

void PrettyPrinter::Visit(Declaration *decl)
{
    os() << "var " << decl->name();

    if (decl->expr()) {
        os() << " = ";
        decl->expr()->Accept(this);
    }
    os() << ";\n";
}

void PrettyPrinter::Visit(DeclarationList *decl_list)
{
    auto &list = decl_list->exprs();

    for (auto &decl : list) {
        decl->Accept(this);
    }
}

void PrettyPrinter::Visit(BlockStatement *stmt)
{
    auto list = stmt->statements();

    os() << "{\n";
    tab()++;
    for (auto &expr : *list) {
        os_tabbed();
        expr->Accept(this);
        os() << ";\n";
    }
    tab()--;
    os() << "}\n";
}

void PrettyPrinter::Visit(ForStatement *stmt)
{
    os() << "for (";
    if (stmt->init())
        stmt->init()->Accept(this);

    if (stmt->condition()) {
        if (stmt->kind() == ForKind::kForIn) {
            os() << " in ";
        } else {
            os() << "; ";
        }
        stmt->condition()->Accept(this);
    } else {
        os() << ";";
    }

    if (stmt->update()) {
        os() << "; ";
        stmt->update()->Accept(this);
    } else {
        os() << ";";
    }
    os() << ")\n";
    stmt->body()->Accept(this);
}

void PrettyPrinter::Visit(WhileStatement *stmt)
{
    os() << "while (";
    stmt->condition()->Accept(this);
    os() << ")";

    stmt->body()->Accept(this);
    os() << "\n";
}

void PrettyPrinter::Visit(DoWhileStatement *stmt)
{
    os() << "do\n";

    stmt->body()->Accept(this);
    os() << "while (";
    stmt->condition()->Accept(this);
    os() << ");\n";
}

void PrettyPrinter::Visit(BreakStatement *stmt)
{
    os() << "break";
    if (stmt->label()) {
        os() << " ";
        stmt->label()->Accept(this);
    }
    os() << ";\n";
}

void PrettyPrinter::Visit(ContinueStatement *stmt)
{
    os() << "continue";
    if (stmt->label()) {
        os() << " ";
        stmt->label()->Accept(this);
    }
    os() << ";\n";
}

void PrettyPrinter::Visit(ThrowStatement *stmt)
{
    os() << "throw ";
    stmt->expr()->Accept(this);
    os() << ";\n";
}

void PrettyPrinter::Visit(TryCatchStatement *stmt)
{
    os() << "try {\n";
    stmt->try_block()->Accept(this);
    os() << "}";
    if (stmt->catch_expr()) {
        os() << " catch (" << stmt->catch_expr()->AsIdentifier()->GetName();
        os() << ") {\n";
        stmt->catch_block()->Accept(this);
        os() << "}";
    }

    if (stmt->finally()) {
        os() << " finally {";
        stmt->finally()->Accept(this);
        os() << "}\n";
    }
}

void PrettyPrinter::Visit(LabelledStatement *stmt)
{
    os() << stmt->label() << ":\n";
    stmt->expr()->Accept(this);
    os() << "\n";
}

void PrettyPrinter::Visit(CaseClauseStatement *stmt)
{
    os() << "case ";
    stmt->clause()->Accept(this);
    os() << ":\n";
    stmt->stmt()->Accept(this);
    os() << "\n";
}

void PrettyPrinter::Visit(SwitchStatement *stmt)
{
    os() << "switch (";
    stmt->expr()->Accept(this);

    os() << ") {\n";
    auto list = stmt->clauses();

    for (auto &clause : *list) {
        clause->Accept(this);
    }

    if (stmt->default_clause()) {
        os() << "default: ";
        stmt->default_clause()->Accept(this);
    }
    os() << "}\n";
}

void PrettyPrinter::Visit(FunctionPrototype *proto)
{
    os() << "function " << proto->GetName() << "(";

    std::string out;
    for (const auto &arg : proto->GetArgs()) {
        out += arg + ", ";
    }

    if (proto->GetArgs().size()) {
        out.pop_back();
        out.pop_back();
        os() << out;
    }
    os() << ")";
}

void PrettyPrinter::Visit(FunctionStatement *stmt)
{
    stmt->proto()->Accept(this);
    os() << " {\n";
    stmt->body()->Accept(this);
    os() << "}\n";
}

void PrettyPrinter::Visit(IfStatement *stmt)
{
    os() << "if (";
    stmt->condition()->Accept(this);
    os() << ")";
    stmt->body()->Accept(this);
}

void PrettyPrinter::Visit(IfElseStatement *stmt)
{
    os_tabbed() << "if (";
    auto tabs = tab();
    tab() = 0;
    stmt->condition()->Accept(this);
    os() << ")";
    tab() = tabs;
    stmt->body()->Accept(this);
    tab() = 0;
    os() << " else ";
    tab() = tabs;
    stmt->els()->Accept(this);
}

void PrettyPrinter::Visit(ReturnStatement *stmt)
{
    os() << "return ";
    if (stmt->expr())
        stmt->expr()->Accept(this);
    os() << ";\n";
}

}
