#include "jast/ast-match.h"
#include "jast/expression.h"
#include "jast/statement.h"

namespace jast {

bool MatchExpressionList(Handle<ExpressionList> a, Handle<ExpressionList> b)
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

bool MatchArrayLiteral(Handle<ArrayLiteral> a, Handle<ArrayLiteral> b)
{
        auto &a_exprs = a->exprs();
        auto &b_exprs = b->exprs();

        if (a_exprs.size() != b_exprs.size())
            return false;

        for (decltype(a_exprs.size()) i = 0; i < a_exprs.size(); i++) {
            if (!FastASTMatcher::match(a_exprs[i], b_exprs[i]))
                return false;
        }

        return true;
}

bool MatchObjectLiteral(Handle<ObjectLiteral> a, Handle<ObjectLiteral> b)
{
    auto &a_object = a->proxy();
    auto &b_object = b->proxy();

    if (a_object.size() != b_object.size())
        return false;

    auto it = b_object.begin();
    for (auto &p : a_object) {
        if ((it = b_object.find(p.first)) != b_object.end()) {
            if (!FastASTMatcher::match(it->second, p.second))
                return false;
        } else {
            return false;
        }
    }

    return true;
}

bool MatchIdentifier(Handle<Identifier> a, Handle<Identifier> b)
{
    return a->GetName() == b->GetName();
}

bool MatchBooleanLiteral(Handle<BooleanLiteral> a, Handle<BooleanLiteral> b)
{
    return a->pred() == b->pred();
}

bool MatchRegExpLiteral(Handle<RegExpLiteral> a, Handle<RegExpLiteral> b)
{
    // TODO;
    return false;
}

bool MatchArgumentList(Handle<ArgumentList> a, Handle<ArgumentList> b)
{
    return MatchExpressionList(a->args(), b->args());
}

bool MatchCallExpression(Handle<CallExpression> a, Handle<CallExpression> b)
{
    return FastASTMatcher::match(a->member(), b->member())
        && FastASTMatcher::match(a->expr(), b->expr());
}

bool MatchMemberExpression(Handle<MemberExpression> a, Handle<MemberExpression> b)
{
    return FastASTMatcher::match(a->member(), b->member())
        && FastASTMatcher::match(a->expr(), b->expr());
}

bool MatchNewExpression(Handle<NewExpression> a, Handle<NewExpression> b)
{
    return FastASTMatcher::match(a->member(), b->member());
}

bool MatchPrefixExpression(Handle<PrefixExpression> a, Handle<PrefixExpression> b)
{
    return (a->op() == b->op()) && (FastASTMatcher::match(a->expr(),
        b->expr()));
}

bool MatchPostfixExpression(Handle<PostfixExpression> a, Handle<PostfixExpression> b)
{
    return (a->op() == b->op()) && (FastASTMatcher::match(a->expr(),
        b->expr()));
}

bool MatchBinaryExpression(Handle<BinaryExpression> a, Handle<BinaryExpression> b)
{
    return (a->op() == b->op())
        && FastASTMatcher::match(a->lhs(), b->lhs())
        && FastASTMatcher::match(a->rhs(), b->rhs());
}

bool MatchAssignExpression(Handle<AssignExpression> a, Handle<AssignExpression> b)
{
    return FastASTMatcher::match(a->lhs(), b->lhs())
        && FastASTMatcher::match(a->rhs(), b->rhs());
}

bool MatchTernaryExpression(Handle<TernaryExpression> a, Handle<TernaryExpression> b)
{
    return FastASTMatcher::match(a->first(), b->first())
        && FastASTMatcher::match(a->second(), b->second())
        && FastASTMatcher::match(a->third(), b->third());
}

bool MatchCommaExpression(Handle<CommaExpression> a, Handle<CommaExpression> b)
{
    return MatchExpressionList(a->exprs(), b->exprs());
}

bool MatchDeclaration(Handle<Declaration> a, Handle<Declaration> b)
{
    return a->name() == b->name()
        && FastASTMatcher::match(a->expr(), b->expr());
}

bool MatchDeclarationList(Handle<DeclarationList> a, Handle<DeclarationList> b)
{
    auto &a_exprs = a->exprs();
    auto &b_exprs = b->exprs();

    for (decltype(a_exprs.size()) i = 0; i < a_exprs.size(); i++) {
        if (!MatchDeclaration(a_exprs[i], b_exprs[i]))
            return false;
    }

    return true;
}

bool MatchBlockStatement(Handle<BlockStatement> a, Handle<BlockStatement> b)
{
    return MatchExpressionList(a->statements(), b->statements());
}

bool MatchForStatement(Handle<ForStatement> a, Handle<ForStatement> b)
{
    return FastASTMatcher::match(a->init(), b->init())
        && FastASTMatcher::match(a->condition(), b->condition())
        && FastASTMatcher::match(a->update(), b->update())
        && FastASTMatcher::match(a->body(), b->body());
}

bool MatchWhileStatement(Handle<WhileStatement> a, Handle<WhileStatement> b)
{
    return FastASTMatcher::match(a->condition(), b->condition())
        && FastASTMatcher::match(a->body(), b->body());
}

bool MatchDoWhileStatement(Handle<DoWhileStatement> a, Handle<DoWhileStatement> b)
{
    return FastASTMatcher::match(a->condition(), b->condition())
        && FastASTMatcher::match(a->body(), b->body());
}

bool MatchBreakStatement(Handle<BreakStatement> a, Handle<BreakStatement> b)
{
    return FastASTMatcher::match(a->label(), b->label());
}

bool MatchContinueStatement(Handle<ContinueStatement> a, Handle<ContinueStatement> b)
{
    return FastASTMatcher::match(a->label(), b->label());
}

bool MatchThrowStatement(Handle<ThrowStatement> a, Handle<ThrowStatement> b)
{
    return FastASTMatcher::match(a->expr(), b->expr());
}

bool MatchTryStatement(Handle<TryCatchStatement> a, Handle<TryCatchStatement> b)
{
    return FastASTMatcher::match(a->try_block(), b->try_block())
        && FastASTMatcher::match(a->catch_expr(), b->catch_expr())
        && FastASTMatcher::match(a->catch_block(), b->catch_block())
        && FastASTMatcher::match(a->finally(), b->finally());
}

bool MatchLabelledStatement(Handle<LabelledStatement> a, Handle<LabelledStatement> b)
{
    return a->label() == b->label() && FastASTMatcher::match(a->expr(),
        b->expr());
}

bool MatchCaseClauseStatement(Handle<CaseClauseStatement> a, Handle<CaseClauseStatement> b)
{
    return FastASTMatcher::match(a->clause(), b->clause())
        && FastASTMatcher::match(a->stmt(), b->stmt());
}

bool MatchClausesList(Handle<ClausesList> a, Handle<ClausesList> b)
{
    if (!a->HasDefaultCase() || !b->HasDefaultCase()
        || a->Size() != b->Size())
        return false;

    for (decltype(a->Size()) i = 0; i < a->Size(); i++) {
        if (!MatchCaseClauseStatement(*(a->begin() + i), *(b->begin() + i)))
            return false;
    }

    if (FastASTMatcher::match(a->def(), b->def()))
        return false;

    return true;
}

bool MatchSwitchStatement(Handle<SwitchStatement> a, Handle<SwitchStatement> b)
{
    return FastASTMatcher::match(a->expr(), b->expr())
        && MatchClausesList(a->clauses(), b->clauses());
}

bool MatchFunctionPrototype(Handle<FunctionPrototype> a, Handle<FunctionPrototype> b)
{
    return a->GetName() == b->GetName()
        && a->GetArgs() == b->GetArgs();
}

bool MatchFunctionStatement(Handle<FunctionStatement> a, Handle<FunctionStatement> b)
{
    return MatchFunctionPrototype(a->proto(), b->proto())
        && FastASTMatcher::match(a->body(), b->body());
}

bool MatchIfStatement(Handle<IfStatement> a, Handle<IfStatement> b)
{
    return FastASTMatcher::match(a->condition(), b->condition())
        && FastASTMatcher::match(a->body(), b->body());
}

bool MatchIfElseStatement(Handle<IfElseStatement> a, Handle<IfElseStatement> b)
{
    return FastASTMatcher::match(a->condition(), b->condition())
        && FastASTMatcher::match(a->body(), b->body())
        && FastASTMatcher::match(a->els(), b->els());
}

bool MatchIfElseStatement(Handle<ReturnStatement> a, Handle<ReturnStatement> b)
{
    return FastASTMatcher::match(a->expr(), b->expr());
}

bool FastASTMatcher::match(Handle<Expression> a, Handle<Expression> b)
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
      return MatchMemberExpression(a->AsMemberExpression(),
                                   b->AsMemberExpression());
    case ASTNodeType::kNewExpression:
      return MatchNewExpression(a->AsNewExpression(), b->AsNewExpression());
    case ASTNodeType::kPrefixExpression:
      return MatchPrefixExpression(a->AsPrefixExpression(),
                                   b->AsPrefixExpression());
    case ASTNodeType::kPostfixExpression:
      return MatchPostfixExpression(a->AsPostfixExpression(),
                                    b->AsPostfixExpression());
    case ASTNodeType::kBinaryExpression:
      return MatchBinaryExpression(a->AsBinaryExpression(),
                                   b->AsBinaryExpression());
    case ASTNodeType::kAssignExpression:
      return MatchAssignExpression(a->AsAssignExpression(),
                                   b->AsAssignExpression());
    case ASTNodeType::kTernaryExpression:
      return MatchTernaryExpression(a->AsTernaryExpression(),
                                    b->AsTernaryExpression());
    case ASTNodeType::kCommaExpression:
      return MatchCommaExpression(a->AsCommaExpression(),
                                  b->AsCommaExpression());
    case ASTNodeType::kDeclaration:
      return MatchDeclaration(a->AsDeclaration(), b->AsDeclaration());
    case ASTNodeType::kDeclarationList:
      return MatchDeclarationList(a->AsDeclarationList(),
                                  b->AsDeclarationList());
    case ASTNodeType::kIfStatement:
      return MatchIfStatement(a->AsIfStatement(), b->AsIfStatement());
    case ASTNodeType::kIfElseStatement:
      return MatchIfElseStatement(a->AsIfElseStatement(),
                                  b->AsIfElseStatement());
    case ASTNodeType::kForStatement:
      return MatchForStatement(a->AsForStatement(), b->AsForStatement());
    case ASTNodeType::kWhileStatement:
      return MatchWhileStatement(a->AsWhileStatement(), b->AsWhileStatement());
    case ASTNodeType::kDoWhileStatement:
      return MatchDoWhileStatement(a->AsDoWhileStatement(),
                                   b->AsDoWhileStatement());
    case ASTNodeType::kLabelledStatement:
      return MatchLabelledStatement(a->AsLabelledStatement(),
                                    b->AsLabelledStatement());
    case ASTNodeType::kBreakStatement:
      return MatchBreakStatement(a->AsBreakStatement(), b->AsBreakStatement());
    case ASTNodeType::kContinueStatement:
      return MatchContinueStatement(a->AsContinueStatement(),
                                    b->AsContinueStatement());
    case ASTNodeType::kSwitchStatement:
      return MatchSwitchStatement(a->AsSwitchStatement(),
                                  b->AsSwitchStatement());
    case ASTNodeType::kCaseClauseStatement:
      return MatchCaseClauseStatement(a->AsCaseClauseStatement(),
                                      b->AsCaseClauseStatement());
    case ASTNodeType::kTryCatchStatement:
      return MatchTryStatement(a->AsTryCatchStatement(),
                               b->AsTryCatchStatement());
    case ASTNodeType::kThrowStatement:
      return MatchThrowStatement(a->AsThrowStatement(), b->AsThrowStatement());
    case ASTNodeType::kBlockStatement:
      return MatchBlockStatement(a->AsBlockStatement(), b->AsBlockStatement());
    case ASTNodeType::kFunctionPrototype:
      return MatchFunctionPrototype(a->AsFunctionPrototype(),
                                    b->AsFunctionPrototype());
    case ASTNodeType::kFunctionStatement:
      return MatchFunctionStatement(a->AsFunctionStatement(),
                                    b->AsFunctionStatement());
    case ASTNodeType::kReturnStatement:
      return FastASTMatcher::match(a->AsReturnStatement()->expr(),
                                   b->AsReturnStatement()->expr());
    default:
        throw std::runtime_error("fatal: unknown type");
    }
    return false;
}

bool LazyASTMatcher::match(Handle<Expression> a, Handle<Expression> b)
{
    // TODO := write code here...
    return false;
}

}
