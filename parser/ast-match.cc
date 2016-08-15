#include "parser/ast-match.h"
#include "parser/expression.h"
#include "parser/statement.h"

namespace grok {
namespace parser {

bool MatchExpressionList(ExpressionList *a, ExpressionList *b)
{
    auto ait = a->begin();
    auto bit = b->begin();

    if (a->Size() != b->Size())
        return false;
    for (size_t i = 0; i < a->Size(); i++)
        if (!FastASTMatcher::match(*(ait + i), *(bit + i)))
            return false;

    return true;
}

bool MatchArrayLiteral(ArrayLiteral *a, ArrayLiteral *b)
{
        auto &a_exprs = a->exprs();
        auto &b_exprs = b->exprs();

        if (a_exprs.size() != b_exprs.size())
            return false;

		for (int i = 0; i < a_exprs.size(); i++) {
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

	auto it = b_object.begin();
    for (auto &p : a_object) {
        if ((it = b_object.find(p.first)) != b_object.end()) {
            if (!FastASTMatcher::match(it->second.get(), p.second.get()))
                return false;
        } else {
            return false;
        }
    }

    return true;
}

bool MatchIdentifier(Identifier *a, Identifier *b)
{
    return a->GetName() == b->GetName();
}

bool MatchBooleanLiteral(BooleanLiteral *a, BooleanLiteral *b)
{
    return a->pred() == b->pred();
}

bool MatchRegExpLiteral(RegExpLiteral *a, RegExpLiteral *b)
{
    // TODO;
    return false;
}

bool MatchArgumentList(ArgumentList *a, ArgumentList *b)
{
    return MatchExpressionList(a->args(), b->args());
}

bool MatchCallExpression(CallExpression *a, CallExpression *b)
{
    return FastASTMatcher::match(a->member(), b->member())
        && FastASTMatcher::match(a->expr(), b->expr());
}

bool MatchMemberExpression(MemberExpression *a, MemberExpression *b)
{
    return FastASTMatcher::match(a->member(), b->member())
        && FastASTMatcher::match(a->expr(), b->expr());
}

bool MatchNewExpression(NewExpression *a, NewExpression *b)
{
    return FastASTMatcher::match(a->member(), b->member());
}

bool MatchPrefixExpression(PrefixExpression *a, PrefixExpression *b)
{
    return (a->op() == b->op()) && (FastASTMatcher::match(a->expr(),
        b->expr()));
}

bool MatchPostfixExpression(PostfixExpression *a, PostfixExpression *b)
{
    return (a->op() == b->op()) && (FastASTMatcher::match(a->expr(),
        b->expr()));
}

bool MatchBinaryExpression(BinaryExpression *a, BinaryExpression *b)
{
    return (a->op() == b->op())
        && FastASTMatcher::match(a->lhs(), b->lhs())
        && FastASTMatcher::match(a->rhs(), b->rhs());
}

bool MatchAssignExpression(AssignExpression *a, AssignExpression *b)
{
    return FastASTMatcher::match(a->lhs(), b->lhs())
        && FastASTMatcher::match(a->rhs(), b->rhs());
}

bool MatchTernaryExpression(TernaryExpression *a, TernaryExpression *b)
{
    return FastASTMatcher::match(a->first(), b->first())
        && FastASTMatcher::match(a->second(), b->second())
        && FastASTMatcher::match(a->third(), b->third());
}

bool MatchCommaExpression(CommaExpression *a, CommaExpression *b)
{
    return MatchExpressionList(a->exprs(), b->exprs());
}

bool MatchDeclaration(Declaration *a, Declaration *b)
{
    return a->name() == b->name()
        && FastASTMatcher::match(a->expr(), b->expr());
}

bool MatchDeclarationList(DeclarationList *a, DeclarationList *b)
{
    auto &a_exprs = a->exprs();
    auto &b_exprs = b->exprs();

    for (int i = 0; i < a_exprs.size(); i++) {
        if (!MatchDeclaration(a_exprs[i].get(), b_exprs[i].get()))
            return false;
    }

    return true;
}

bool MatchBlockStatement(BlockStatement *a, BlockStatement *b)
{
    return MatchExpressionList(a->statements(), b->statements());
}

bool MatchForStatement(ForStatement *a, ForStatement *b)
{
    return FastASTMatcher::match(a->init(), b->init())
        && FastASTMatcher::match(a->condition(), b->condition())
        && FastASTMatcher::match(a->update(), b->update())
        && FastASTMatcher::match(a->body(), b->body());
}

bool MatchWhileStatement(WhileStatement *a, WhileStatement *b)
{
    return FastASTMatcher::match(a->condition(), b->condition())
        && FastASTMatcher::match(a->body(), b->body());
}

bool MatchDoWhileStatement(DoWhileStatement *a, DoWhileStatement *b)
{
    return FastASTMatcher::match(a->condition(), b->condition())
        && FastASTMatcher::match(a->body(), b->body());
}

bool MatchBreakStatement(BreakStatement *a, BreakStatement *b)
{
    return FastASTMatcher::match(a->label(), b->label());
}

bool MatchContinueStatement(ContinueStatement *a, ContinueStatement *b)
{
    return FastASTMatcher::match(a->label(), b->label());
}

bool MatchThrowStatement(ThrowStatement *a, ThrowStatement *b)
{
    return FastASTMatcher::match(a->expr(), b->expr());
}

bool MatchTryStatement(TryCatchStatement *a, TryCatchStatement *b)
{
    return FastASTMatcher::match(a->try_block(), b->try_block())
        && FastASTMatcher::match(a->catch_expr(), b->catch_expr())
        && FastASTMatcher::match(a->catch_block(), b->catch_block())
        && FastASTMatcher::match(a->finally(), b->finally());
}

bool MatchLabelledStatement(LabelledStatement *a, LabelledStatement *b)
{
    return a->label() == b->label() && FastASTMatcher::match(a->expr(),
        b->expr());
}

bool MatchCaseClauseStatement(CaseClauseStatement *a, CaseClauseStatement *b)
{
    return FastASTMatcher::match(a->clause(), b->clause())
        && FastASTMatcher::match(a->stmt(), b->stmt());
}

bool MatchClausesList(ClausesList *a, ClausesList *b)
{
    if (!a->HasDefaultCase() || !b->HasDefaultCase()
        || a->Size() != b->Size())
        return false;

    for (auto i = 0; i < a->Size(); i++) {
        if (!MatchCaseClauseStatement(*(a->begin() + i), *(b->begin() + i)))
            return false;
    }

    if (FastASTMatcher::match(a->def(), b->def()))
        return false;

    return true;
}

bool MatchSwitchStatement(SwitchStatement *a, SwitchStatement *b)
{
    return FastASTMatcher::match(a->expr(), b->expr())
        && MatchClausesList(a->clauses(), b->clauses());
}

bool MatchFunctionPrototype(FunctionPrototype *a, FunctionPrototype *b)
{
    return a->GetName() == b->GetName()
        && a->GetArgs() == b->GetArgs();
}

bool MatchFunctionStatement(FunctionStatement *a, FunctionStatement *b)
{
    return MatchFunctionPrototype(a->proto(), b->proto())
        && FastASTMatcher::match(a->body(), b->body());
}

bool MatchIfStatement(IfStatement *a, IfStatement *b)
{
    return FastASTMatcher::match(a->condition(), b->condition())
        && FastASTMatcher::match(a->body(), b->body());
}

bool MatchIfElseStatement(IfElseStatement *a, IfElseStatement *b)
{
    return FastASTMatcher::match(a->condition(), b->condition())
        && FastASTMatcher::match(a->body(), b->body())
        && FastASTMatcher::match(a->els(), b->els());
}

bool MatchIfElseStatement(ReturnStatement *a, ReturnStatement *b)
{
    return FastASTMatcher::match(a->expr(), b->expr());
}

bool FastASTMatcher::match(Expression *a, Expression *b)
{
    if (!a && !b)
        return true;
    else if (!a || !b)
        return false;
    else if (a->type() != b->type())
        return false;
    switch (a->type()) {
    case ASTNodeType::kNullLiteral:
    case ASTNodeType::kUndefinedLiteral:
    case ASTNodeType::kThisHolder:
        return true;

    case ASTNodeType::kIntegralLiteral:
        return a->AsIntegralLiteral()->value() == b->AsIntegralLiteral()->value();

    case ASTNodeType::kStringLiteral:
        return a->AsStringLiteral()->string() == b->AsStringLiteral()->string();

    case ASTNodeType::kArrayLiteral:
        return MatchArrayLiteral(a->AsArrayLiteral(), b->AsArrayLiteral());
    case ASTNodeType::kObjectLiteral:
        return MatchObjectLiteral(a->AsObjectLiteral(), b->AsObjectLiteral());
    case ASTNodeType::kIdentifier:
        return MatchIdentifier(a->AsIdentifier(), b->AsIdentifier());
    case ASTNodeType::kBooleanLiteral:
        return MatchBooleanLiteral(a->AsBooleanLiteral(), b->AsBooleanLiteral());
    case ASTNodeType::kRegExpLiteral:
        return MatchRegExpLiteral(a->AsRegExpLiteral(), b->AsRegExpLiteral());
    case ASTNodeType::kArgumentList:
        return MatchArgumentList(a->AsArgumentList(), b->AsArgumentList());
    case ASTNodeType::kCallExpression:
        return MatchCallExpression(a->AsCallExpression(), b->AsCallExpression());
    case ASTNodeType::kMemberExpression:
        return MatchMemberExpression(a->AsMemberExpression(), b->AsMemberExpression());
    case ASTNodeType::kNewExpression:
        return MatchNewExpression(a->AsNewExpression(), b->AsNewExpression());
    case ASTNodeType::kPrefixExpression:
        return MatchPrefixExpression(a->AsPrefixExpression(), b->AsPrefixExpression());
    case ASTNodeType::kPostfixExpression:
        return MatchPostfixExpression(a->AsPostfixExpression(), b->AsPostfixExpression());
    case ASTNodeType::kBinaryExpression:
        return MatchBinaryExpression(a->AsBinaryExpression(), b->AsBinaryExpression());
    case ASTNodeType::kAssignExpression:
        return MatchAssignExpression(a->AsAssignExpression(), b->AsAssignExpression());
    case ASTNodeType::kTernaryExpression:
        return MatchTernaryExpression(a->AsTernaryExpression(), b->AsTernaryExpression());
    case ASTNodeType::kCommaExpression:
        return MatchCommaExpression(a->AsCommaExpression(), b->AsCommaExpression());
    case ASTNodeType::kDeclaration:
        return MatchDeclaration(a->AsDeclaration(), b->AsDeclaration());
    case ASTNodeType::kDeclarationList:
        return MatchDeclarationList(a->AsDeclarationList(), b->AsDeclarationList());
    case ASTNodeType::kIfStatement:
        return MatchIfStatement(a->AsIfStatement(), b->AsIfStatement());
    case ASTNodeType::kIfElseStatement:
        return MatchIfElseStatement(a->AsIfElseStatement(), b->AsIfElseStatement());
    case ASTNodeType::kForStatement:
        return MatchForStatement(a->AsForStatement(), b->AsForStatement());
    case ASTNodeType::kWhileStatement:
        return MatchWhileStatement(a->AsWhileStatement(), b->AsWhileStatement());
    case ASTNodeType::kDoWhileStatement:
        return MatchDoWhileStatement(a->AsDoWhileStatement(), b->AsDoWhileStatement());
    case ASTNodeType::kLabelledStatement:
        return MatchLabelledStatement(a->AsLabelledStatement(), b->AsLabelledStatement());
    case ASTNodeType::kBreakStatement:
        return MatchBreakStatement(a->AsBreakStatement(), b->AsBreakStatement());
    case ASTNodeType::kContinueStatement:
        return MatchContinueStatement(a->AsContinueStatement(), b->AsContinueStatement());
    case ASTNodeType::kSwitchStatement:
        return MatchSwitchStatement(a->AsSwitchStatement(), b->AsSwitchStatement());
    case ASTNodeType::kCaseClauseStatement:
        return MatchCaseClauseStatement(a->AsCaseClauseStatement(), b->AsCaseClauseStatement());
    case ASTNodeType::kTryCatchStatement:
        return MatchTryStatement(a->AsTryCatchStatement(), b->AsTryCatchStatement());
    case ASTNodeType::kThrowStatement:
        return MatchThrowStatement(a->AsThrowStatement(), b->AsThrowStatement());
    case ASTNodeType::kBlockStatement:
        return MatchBlockStatement(a->AsBlockStatement(), b->AsBlockStatement());
    case ASTNodeType::kFunctionPrototype:
        return MatchFunctionPrototype(a->AsFunctionPrototype(), b->AsFunctionPrototype());
    case ASTNodeType::kFunctionStatement:
        return MatchFunctionStatement(a->AsFunctionStatement(), b->AsFunctionStatement());
    case ASTNodeType::kReturnStatement:
        return FastASTMatcher::match(a->AsReturnStatement()->expr(),
            b->AsReturnStatement()->expr());
    default:
        throw std::runtime_error("fatal: unknown type");
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
