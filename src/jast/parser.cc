#include "jast/parser.h"
#include "jast/ast-builder.h"
#include "jast/token.h"

#include <sstream>

namespace jast {

#define EXPECT(tok)     \
    do {    \
        if (peek() != tok)  \
            throw SyntaxError(lex()->currentToken(),    \
                        String("expected a ") + Token::str(tok)); \
        advance();  \
    } while (0)

bool IsAssign(TokenType tok)
{
    return tok >= ASSIGN && tok <= ASSIGN_MOD;
}

Parser::~Parser()
{
}

String Parser::GetStringLiteral()
{
    return lex()->currentToken().view();
}

double Parser::ParseNumber(const Token &token)
{
    try {
        double num = std::stod(token.view());
        return num;
    } catch(std::exception &e) {
        throw SyntaxError(token, "invalid number (" + token.view() + ")");
    }
}

String Parser::GetIdentifierName()
{
    return lex()->currentToken().view();
}

TokenType Parser::peek()
{
    return lex()->peek();
}

void Parser::advance()
{
    lex()->advance();
}

Expression* Parser::ParseArrayLiteral()
{
    ProxyArray exprs;
    // eat '['
    advance();
    auto tok = peek();

    if (tok == RBRACK) {
        // done
        return builder()->NewArrayLiteral(std::move(exprs));
    }
    while (true) {
        auto one = ParseAssignExpression();
        exprs.push_back(std::unique_ptr<Expression>(one));

        tok = peek();
        
        if (tok == RBRACK)
            break;
        EXPECT(COMMA); 
    }

    return builder()->NewArrayLiteral(std::move(exprs));
}

Expression* Parser::ParseObjectLiteral()
{
    ProxyObject proxy;

    // eat the left brace '{'
    advance();
    auto tok = peek();

    // if next tok is '}' then nothing to be done
    if (tok == RBRACE) {
        return builder()->NewObjectLiteral(std::move(proxy));
    }

    std::string name;
    Expression* prop;
    while (true) {
        tok = peek();
        if (tok == STRING) {
            name = GetStringLiteral();
        } else if (tok == IDENTIFIER || IsKeyword(tok) || tok == NUMBER) {
            name = lex()->currentToken().view();
        } else {
            throw SyntaxError(lex()->currentToken(),
                        "expected an Identifier or a string");
        }

        advance();
        EXPECT(COLON);
        prop = ParseAssignExpression();
        proxy[name] = std::unique_ptr<Expression>(prop);

        // next token should be a ',' or '}'
        tok = peek();
        if (tok == RBRACE)
            break;
        if (tok != COMMA)
            throw SyntaxError(lex()->currentToken(), "expected a ',' or '}'");
        advance();
    }

    return builder()->NewObjectLiteral(std::move(proxy));
}

Expression* Parser::ParsePrimary()
{
    auto tok = peek();
    Expression* result;

    if (tok == NULL_LITERAL) {
        result = builder()->NewNullLiteral();
    } else if (tok == NUMBER) {
        result = builder()->NewIntegralLiteral(
                                        ParseNumber(lex()->currentToken()));
    } else if (tok == TEMPLATE) {
        result = builder()->NewTemplateLiteral(lex()->currentToken().view());
    } else if (tok == REGEX) {
        std::string regex = lex()->currentToken().view();
        auto pos = regex.rfind("$");
        auto flags = regex.substr(pos);
        std::vector<RegExpFlags> fs;
        for (auto &flag : flags) {
            if (flag == 'g') {
                fs.push_back(RegExpFlags::kGlobal);
            } else if (flag == 'i') {
                fs.push_back(RegExpFlags::kIgnoreCase);
            } else if (flag == 'm') {
                fs.push_back(RegExpFlags::kMultiline);
            } else if (flag == 'y') {
                fs.push_back(RegExpFlags::kSticky);
            } else if (flag == 'u') {
                fs.push_back(RegExpFlags::kUnicode);
            }
        }
        result = builder()->NewRegExpLiteral(regex.substr(0, pos), fs);
    } else if (tok == STRING) {
        result = builder()->NewStringLiteral(lex()->currentToken().view());
    } else if (tok == IDENTIFIER) {
        result = builder()->NewIdentifier(lex()->currentToken().view());
    } else if (tok == TRUE_LITERAL) {
        result = builder()->NewBooleanLiteral(true);
    } else if (tok == FALSE_LITERAL) {
        result = builder()->NewBooleanLiteral(false);
    } else if (tok == THIS) {
        result = builder()->NewThisHolder();
    } else if (tok == LPAREN) {
        advance();    // eat '('
        result = ParseCommaExpression();
        tok = peek();

        if (tok != RPAREN)
            throw SyntaxError(lex()->currentToken(), "expected a ')'");
    } else if (tok == LBRACK) {
        result = ParseArrayLiteral();
    } else if (tok == LBRACE) {
        result = ParseObjectLiteral();
    } else if (tok == FUNCTION) {
        result = ParseFunctionStatement();
        return result;
    } else {
        throw SyntaxError(lex()->currentToken(), "expected a primary expression");
    }

    advance();
    return result;
}

Expression* Parser::ParseDotExpression()
{
    // eat the '.'
    advance();

    auto tok = peek();

    // this token should be a valid identifier
    if (tok != IDENTIFIER && !IsKeyword(tok))
        throw SyntaxError(lex()->currentToken(), "expected a valid identifier");
    auto name = GetIdentifierName();

    auto ident = builder()->NewIdentifier(name);
    advance();
    return ident;
}

Expression* Parser::ParseIndexExpression()
{
    // eat the '['
    advance();
    auto expr = ParseAssignExpression();
    if (lex()->peek() != RBRACK)
        throw SyntaxError(lex()->currentToken(), "expected a ']'");

    advance(); // consumex ']'
    return expr;
}

// MemberExpression :
//    PrimaryExpression
//    FunctionExpression
//    MemberExpression [ Expression ]
//    MemberExpression . IdentifierName
//    new MemberExpression Arguments

// NewExpression ::
//   ('new')+ MemberExpression

// Examples of new expression:
// new foo.bar().baz means (new (foo.bar)()).baz
// new foo()() means (new foo())()
// new new foo()() means (new (new foo())())
// new new foo means new (new foo)
// new new foo() means new (new foo())
// new new foo().bar().baz means (new (new foo()).bar()).baz
Expression *Parser::ParseMemberExpression()
{
    Expression *primary = nullptr;
    if (peek() == TokenType::NEW) {
        advance();
        primary = builder()->NewNewExpression(ParseMemberExpression());
    } else {
        primary = ParsePrimary();
    }

    // if next token is neither '[' or '.'
    if (peek() != LBRACK && peek() != PERIOD && peek() != LPAREN)
        return primary;

    Expression *temp;
    Expression *member = primary;
    MemberAccessKind kind;
    while (true) {
        if (peek() == PERIOD) {
            temp = ParseDotExpression();
            kind = MemberAccessKind::kDot;
        } else if (peek() == LBRACK) {
            temp = ParseIndexExpression();
            kind = MemberAccessKind::kIndex;
        } else if (peek() == LPAREN) {
            auto args = ParseArgumentList();
            temp = builder()->NewArgumentList(args);
            kind = MemberAccessKind::kCall;
        } else {
            break;
        }
        member = builder()->NewMemberExpression(kind, member, temp);
    }

    return member;
}

ExpressionList *Parser::ParseArgumentList()
{
    ExpressionList* exprs = builder()->NewExpressionList();

    auto tok = peek();
    EXPECT(LPAREN);

    tok = peek();
    if (tok == RPAREN) {
        advance();
        return { };
    }

    while (true) {
        auto one = ParseAssignExpression();
        exprs->Insert(one);

        tok = peek();
        if (tok == RPAREN)
            break;
        if (tok != COMMA)
            throw SyntaxError(lex()->currentToken(), "expected a ',' or ')'");
        advance();
    }

    advance(); // eat the last ')'
    return exprs;    
}

// CallExpression :
//      MemberExpression Arguments
//      CallExpression Arguments
//      CallExpression [ Expression ]
//      CallExpression . IdentifierName
Expression *Parser::ParseCallExpression()
{
    auto func = ParseMemberExpression();
    auto tok = peek();

    if (tok != PERIOD && tok != LBRACK && tok != LPAREN)
        return func;
    Expression *member = func, *temp = nullptr;
    MemberAccessKind kind;

    while (true) {
        tok = peek();
        if (tok == PERIOD) {
            temp = ParseDotExpression();
            kind = MemberAccessKind::kDot;
        } else if (tok == LBRACK) {
            temp = ParseIndexExpression();
            kind = MemberAccessKind::kIndex;
        } else if (tok == LPAREN) {
            auto args = ParseArgumentList();
            temp = builder()->NewArgumentList(args);
            kind = MemberAccessKind::kCall;
        } else {
            break;
        }
        member = builder()->NewCallExpression(kind, member, temp);
    }

    return member;
}

// NewExpression :
//      MemberExpression
//      new NewExpression
Expression* Parser::ParseNewExpression()
{
    if (peek() != NEW) {
        return ParseMemberExpression();
    }
    advance(); // eat new
    auto member = ParseNewExpression();
    return builder()->NewNewExpression(member);
}

// LeftHandSideExpression :
//      NewExpression
//      CallExpression
Expression *Parser::ParseLeftHandSideExpression()
{
    if (peek() == NEW) {
        return ParseNewExpression();
    } else {
        return ParseCallExpression();
    }
}

PrefixOperation MapTokenWithPrefixOperator(Token &tok)
{
    switch(tok.type()) {
        case INC:       return PrefixOperation::kIncrement;
        case DEC:       return PrefixOperation::kDecrement;
        case TYPEOF:    return PrefixOperation::kTypeOf;
        case DELETE:    return PrefixOperation::kDelete;
        case BIT_NOT:   return PrefixOperation::kBitNot;
        case NOT:       return PrefixOperation::kNot;
        case VOID:      return PrefixOperation::kVoid;
        default:    throw SyntaxError(tok, "unrecognised token");
    }
}

Expression* Parser::ParseUnaryExpression()
{
    // UnaryExpression :
    //     PostfixExpression
    //     delete UnaryExpression
    //     void UnaryExpression
    //     typeof UnaryExpression
    //     ++ UnaryExpression
    //     -- UnaryExpression
    //     + UnaryExpression
    //     - UnaryExpression
    //     ~ UnaryExpression
    //     ! UnaryExpression
    auto tok = peek();

    if (tok == ADD) {
        advance();
        // convert + (Expr) to Expr * 1
        return builder()->NewBinaryExpression(BinaryOperation::kMultiplication,
            ParseUnaryExpression(), builder()->NewIntegralLiteral(1.0));
    } else if (tok == SUB) {
        advance();

        // similarly for `-Expr` to `Expr * -1` 
        return builder()->NewBinaryExpression(BinaryOperation::kMultiplication,
            ParseUnaryExpression(), builder()->NewIntegralLiteral(-1.0));
    } else if (tok == INC || tok == DEC || tok == NOT || tok == BIT_NOT
        || tok == TYPEOF || tok == DELETE || tok == VOID) {
        auto token = lex()->currentToken();
        lex()->advance();

        return builder()->NewPrefixExpression(MapTokenWithPrefixOperator(token),
                                              ParseUnaryExpression());
    }

    // PostfixExpression :
    //      LeftHandSideExpression
    //      LeftHandSideExpression [no LineTerminator here] ++
    //      LeftHandSideExpression [no LineTerminator here] --
    auto left = ParseLeftHandSideExpression();

    tok = peek();
    if (tok == INC) {
        advance();
        return builder()->NewPostfixExpression(PostfixOperation::kIncrement,
            left);
    } else if (tok == TokenType::DEC) {
        advance();
        return builder()->NewPostfixExpression(PostfixOperation::kDecrement,
            left);
    } else {
        return left;
    }
}

BinaryOperation MapBinaryOperator(Token &tok)
{
    switch(tok.type()) {
        case ADD:           return BinaryOperation::kAddition;
        case SUB:           return BinaryOperation::kSubtraction;
        case MUL:           return BinaryOperation::kMultiplication;
        case DIV:           return BinaryOperation::kDivision;
        case MOD:           return BinaryOperation::kMod;
        case SHL:           return BinaryOperation::kShiftLeft;
        case SAR:           return BinaryOperation::kShiftRight;
        case SHR:           return BinaryOperation::kShiftZeroRight;
        case LT:           return BinaryOperation::kLessThan;
        case GT:           return BinaryOperation::kGreaterThan;
        case LTE:           return BinaryOperation::kLessThanEqual;
        case GTE:           return BinaryOperation::kGreaterThanEqual;
        case EQ:           return BinaryOperation::kEqual;
        case NE:           return BinaryOperation::kNotEqual;
        case EQ_STRICT:           return BinaryOperation::kStrictEqual;
        case NE_STRICT:           return BinaryOperation::kStrictNotEqual;
        case AND:           return BinaryOperation::kAnd;
        case OR:           return BinaryOperation::kOr;
        case BIT_AND:           return BinaryOperation::kBitAnd;
        case BIT_OR:           return BinaryOperation::kBitOr;
        case BIT_XOR:           return BinaryOperation::kBitXor;
        case INSTANCEOF:           return BinaryOperation::kInstanceOf;
        case IN:            return BinaryOperation::kIn;
        default:       throw SyntaxError(tok,
                                    "unexpected token as binary operator");
    }
}

/// reference for this function ::= llvm/examples/Kaleidoscope/chapter3/toy.cpp
/// if you are unable to understand the function just imagine you are 
/// parsing 2 + 3 * 5 - 6 / 7, (I too used that as a reference)
Expression* Parser::ParseBinaryExpressionRhs(int prec, Expression* lhs)
{
    while (true) {
        int tokprec = Token::precedence(peek());

        if (tokprec < prec) {
            return lhs;
        }

        // now we definitely have a binary operator
        auto op =  MapBinaryOperator(lex()->currentToken());
        advance();

        auto rhs = ParseUnaryExpression();

        auto nextprec = Token::precedence(peek());
        if (tokprec < nextprec) {
            rhs = ParseBinaryExpressionRhs(tokprec + 1, rhs);
        }


        // merge lhs and rhs
        lhs = builder()->NewBinaryExpression(op, lhs, rhs);
    }
}

Expression* Parser::ParseBinaryExpression()
{
    auto lhs = ParseUnaryExpression();

    // parse the rhs, if any
    return ParseBinaryExpressionRhs(3, lhs);
}

Expression* Parser::ParseAssignExpression()
{
    auto lhs = ParseTernaryExpression();
    auto tok = peek();

    if (!IsAssign(tok))
        return lhs;

    // TODO: information about what kind of assignment is done here should
    //  be stored here. (in the AST?)
    advance();
    auto rhs = ParseAssignExpression();
    return builder()->NewAssignExpression((lhs), (rhs));
}

Expression* Parser::ParseTernaryExpression()
{
    auto first = ParseBinaryExpression();

    auto tok = peek();
    if (tok != CONDITIONAL) {
        return first;
    }

    // now we're parsing conditional expression
    // eat '?'
    advance();
    auto second = ParseAssignExpression();

    tok = peek();
    if (tok != COLON) {
        throw SyntaxError(lex()->currentToken(), "expected a ':'");
    }

    // eat ':'
    advance();
    auto third = ParseAssignExpression();

    return builder()->NewTernaryExpression(first, second, third);
}

Expression* Parser::ParseCommaExpression()
{
    auto one = ParseAssignExpression();
    auto tok = lex()->peek();

    // if we have a comma ',', then we definitely have to parse
    // expr of type (expr, expr*)
    if (tok != COMMA)
        return one;

    ExpressionList *exprs = builder()->NewExpressionList();
    exprs->Insert(one);

    // loop until we don't find a ','
    while (true) {
        advance();
        one = ParseAssignExpression();
        exprs->Insert(one);

        tok = peek();
        if (tok != COMMA)
            break;
    }

    return builder()->NewCommaExpression(exprs);
}

Expression *Parser::ParseExpression()
{
    return ParseCommaExpression();
}

Expression* Parser::ParseExpressionOptional()
{
    auto tok = peek();

    if (tok == SEMICOLON) {
        return builder()->NewUndefinedLiteral();
    } else {
        return ParseExpression();
    }
}

Expression* Parser::ParseElseBranch()
{
    // eat 'else'
    advance();
    return ParseStatement();
}

Expression* Parser::ParseIfStatement()
{
    Expression* result;

    // eat 'if'
    advance();

    auto tok = peek();
    EXPECT(LPAREN);

    // parse the condition of if statement
    auto condition = ParseCommaExpression();
    EXPECT(RPAREN);

    // parse the body of 'if'
    auto body = ParseStatement();

    tok = peek();
    if (tok == ELSE) {
        result = builder()->NewIfElseStatement(condition, body,
            ParseElseBranch());
    } else {
        result = builder()->NewIfStatement(condition, body);
    }

    return result;
}

Expression* Parser::ParseForInStatement(Expression *inexpr)
{
    EXPECT(RPAREN);

        // parse 'for (x = 10; x < 100; x = x + 1) >>rest<<...' part
    auto body = ParseStatement();
    return builder()->NewForStatement(ForKind::kForIn,
        inexpr, nullptr, nullptr, body);
}

Expression* Parser::ParseForStatement()
{
    // eat 'for'
    advance();
    auto tok = peek();
    EXPECT(LPAREN);

    // parse 'for ( >>this<< ;...' part
    auto init = ParseExpressionOptional();

    if (init && init->IsBinaryExpression()) {
        auto op = init->AsBinaryExpression()->op();
        if (op == BinaryOperation::kIn) {
            return ParseForInStatement(init);
        }
    }
    EXPECT(SEMICOLON);

    Expression* condition;
    if (peek() == SEMICOLON) {
        condition = builder()->NewBooleanLiteral(true);
    } else {
        // parse 'for (x = 10; >>this<< ...' part
        condition = ParseCommaExpression();
    }

    tok = peek();
    if (tok != SEMICOLON)
        throw SyntaxError(lex()->currentToken(), "expected a ';'");
    advance();

    Expression* update;
    if (peek() != RPAREN) {
        // parse 'for (x = 10; x < 100; >>this<<...' part
        update = ParseCommaExpression();
    } else {
        update = builder()->NewUndefinedLiteral();
    }

    tok = peek();
    if (tok != RPAREN)
        throw SyntaxError(lex()->currentToken(), "expected a ')'");
    advance();

    // parse 'for (x = 10; x < 100; x = x + 1) >>rest<<...' part
    auto body = ParseStatement();
    return builder()->NewForStatement(ForKind::kForOf,
        init, condition, update, body);
}

Expression* Parser::ParseWhileStatement()
{
    advance(); // eat 'while'

    auto tok = peek();

    if (tok != LPAREN) {
        throw SyntaxError(lex()->currentToken(), "expected a '('");
    }
    advance();

    auto condition = ParseCommaExpression();
    tok = peek();
    if (tok != RPAREN)
        throw SyntaxError(lex()->currentToken(), "expected a ')'");

    advance();
    auto body = ParseStatement();

    return builder()->NewWhileStatement(condition, body);
}

Expression* Parser::ParseDoWhileStatement()
{
    advance(); // eat 'do'
    auto body = ParseStatement();

    auto tok = peek();
    if (tok != WHILE)
        throw SyntaxError(lex()->currentToken(), "expected 'while'");
    advance();

    tok = peek();
    if (tok != LPAREN) {
        throw SyntaxError(lex()->currentToken(), "expected a '('");
    }
    advance();

    auto condition = ParseCommaExpression();
    tok = peek();
    if (tok != RPAREN) {
        throw SyntaxError(lex()->currentToken(), "expected a ')'");
    }
    advance();

    tok = peek();
    if (tok != SEMICOLON)
        throw SyntaxError(lex()->currentToken(), "expected a ';'");
    advance();

    return builder()->NewDoWhileStatement(condition, body);
}

std::vector<std::string> Parser::ParseParameterList()
{
    auto tok = peek();
    auto result = std::vector<std::string>();

    if (tok != LPAREN)
        throw SyntaxError(lex()->currentToken(), "expected a '('");
    advance();

    // check for ')'
    tok = peek();
    if (tok == RPAREN) {
        advance();
        return { }; // return an empty vector
    }

    while (true) {
        tok = peek();

        if (tok != IDENTIFIER) 
            throw SyntaxError(lex()->currentToken(), "expected an identifier");

        result.push_back(GetIdentifierName());
        advance();

        tok = peek();
        if (tok == RPAREN)
            break;

        if (tok != COMMA)
            throw SyntaxError(lex()->currentToken(), "expected a ',' or ')'");
        advance();
    }

    // eat the ')'
    advance();
    return result;
}

FunctionPrototype* Parser::ParseFunctionPrototype()
{
    // eat 'function'   
    advance();
    auto tok = peek();

    std::string name;
    if (tok == IDENTIFIER) {
        name = GetIdentifierName();
        // eat the IDENT
        advance();
    }

    // parse the argument list
    auto args = ParseParameterList();
    return builder()->NewFunctionPrototype(name, std::move(args))
                ->AsFunctionPrototype();
}

Expression* Parser::ParseFunctionStatement()
{
    auto proto = ParseFunctionPrototype();
    auto body = ParseStatement();

    return builder()->NewFunctionStatement(proto, body);
}

Expression* Parser::ParseBlockStatement()
{
    ExpressionList *stmts = builder()->NewExpressionList();
    advance(); // eat '{'

    while (true) {
        auto tok = peek();
        if (tok == RBRACE)
            break;

        auto stmt = ParseStatement();
        stmts->Insert(stmt);
    }

    advance(); // eat '}'
    return builder()->NewBlockStatement(stmts);
}

Expression* Parser::ParseReturnStatement()
{
    advance(); // eat 'return'
    auto tok = peek();

    if (tok == SEMICOLON) {
        return builder()->NewReturnStatement(nullptr);
    }

    auto expr = ParseAssignExpression();
    tok = peek();

    if (tok != SEMICOLON)
        throw SyntaxError(lex()->currentToken(), "expected a ';'");
    advance();
    return builder()->NewReturnStatement(expr);
}

Declaration* Parser::ParseDeclaration()
{
    auto tok = peek();
    if (tok != IDENTIFIER) {
        throw SyntaxError(lex()->currentToken(), "expected an identifier");
    }
    std::string name = GetIdentifierName();
    advance();

    tok = peek();
    if (tok == SEMICOLON || tok == COMMA) {
        return builder()->factory()->NewDeclaration(
            builder()->locator()->loc(), name);
    } else if (tok != ASSIGN) {
        throw SyntaxError(lex()->currentToken(), "expected a '='");
    }
    advance();
    return builder()->factory()->NewDeclaration(
            builder()->locator()->loc(), name, ParseAssignExpression());
}

Expression* Parser::ParseVariableStatement()
{
    advance();    // eat 'var'

    std::vector<std::unique_ptr<Declaration>> decl_list;
    while (true) {
        decl_list.push_back(std::unique_ptr<Declaration>(ParseDeclaration()));

        auto tok = peek();
        if (tok == SEMICOLON)
            break;
        else if (tok != COMMA)
            throw SyntaxError(lex()->currentToken(), "expected a ',' or ';'");
        advance(); // eat ','
    }

    advance(); // eat ';'
    return builder()->factory()->NewDeclarationList(
            builder()->locator()->loc(), std::move(decl_list));
}

Expression *Parser::ParseCaseBlock()
{
    advance();

    // TODO ::= make it more abstract. use ParseExpression
    Expression *clause = ParseAssignExpression();
    EXPECT(COLON);
    Expression *stmt = ParseStatement();

    return builder()->NewCaseClauseStatement(clause, stmt);
}

Expression *Parser::ParseDefaultClause()
{
    advance();

    EXPECT(COLON);
    Expression *stmt = ParseStatement();
    return stmt;
}

Expression* Parser::ParseSwitchStatement()
{
    advance();
    EXPECT(LPAREN);

    Expression *expr = ParseAssignExpression();
    EXPECT(RPAREN);
    EXPECT(LBRACE);

    bool has_default = false;
    ClausesList *list = builder()->NewClausesList();
    while (true) {
        Expression *temp = nullptr;
        if (peek() == CASE) {
            temp = ParseCaseBlock();
            list->PushCase(temp->AsCaseClauseStatement());
        } else if (peek() == DEFAULT) {
            if (has_default) {
                throw SyntaxError(lex()->currentToken(),
                        "switch statement has already has one default case");
            }
            temp = ParseDefaultClause();
            list->SetDefaultCase(temp);
            has_default = true;
        } else if (peek() != RBRACE) {
            throw SyntaxError(lex()->currentToken(), "expected a '}'");
        } else {
            break;
        }
    }

    return builder()->NewSwitchStatement(expr, list);
}

Expression* Parser::ParseBreakStatement()
{
    advance();
    Expression *label = nullptr;

    if (peek() == IDENTIFIER) {
        label = builder()->NewIdentifier(GetIdentifierName());
    }

    return builder()->NewBreakStatement(label);
}

Expression* Parser::ParseContinueStatement()
{

    advance();
    Expression *label = nullptr;

    if (peek() == IDENTIFIER) {
        label = builder()->NewIdentifier(GetIdentifierName());
    }

    return builder()->NewContinueStatement(label);
}

Expression* Parser::ParseTryCatchStatement()
{
    Expression *try_block = nullptr;
    Expression *catch_expr = nullptr;
    Expression *catch_block = nullptr;
    Expression *finally = nullptr;
    EXPECT(TRY);

    try_block = ParseBlockStatement();
    
    if (peek() == CATCH) {
        advance();
        EXPECT(LPAREN);
        catch_expr = ParseExpression();
        EXPECT(RPAREN);
        catch_block = ParseBlockStatement();
    }
    if (peek() == FINALLY) {
        advance();
        finally = ParseBlockStatement();
    }
    return builder()->NewTryCatchStatement(try_block, catch_expr, catch_block,
        finally);
}

// ThrowStatement :
//      throw [no line terminator] Expression ;
Expression* Parser::ParseThrowStatement()
{
    EXPECT(THROW);

    Expression *expr = ParseExpression();

    return builder()->NewThrowStatement(expr);
}

Expression* Parser::ParseStatement()
{
    auto tok = peek();

    switch (tok) {
    default:
    {
        auto result = ParseExpressionOptional();
        tok = peek();

        if (tok != SEMICOLON)
            throw SyntaxError(lex()->currentToken(), "expected a ';'");
        advance();
        return result;
    }

    case IF:
        return ParseIfStatement();  
    case FOR:
        return ParseForStatement();
    case FUNCTION:
        return ParseFunctionStatement();
    case LBRACE:
        return ParseBlockStatement();
    case RETURN:
        return ParseReturnStatement();
    case WHILE:
        return ParseWhileStatement();
    case DO:
        return ParseDoWhileStatement();
    case VAR:
        return ParseVariableStatement();
    case SWITCH:
        return ParseSwitchStatement();
    case BREAK:
        return ParseBreakStatement();
    case CONTINUE:
        return ParseContinueStatement();
    case TRY:
        return ParseTryCatchStatement();
    case THROW:
        return ParseThrowStatement();
    }
}

std::string CreateLLVMLikePointer(size_t pos, size_t len)
{
    std::string result;
    if (pos >= len) {
        result += std::string(len, '~');
        result += '^';
    } else {
        result += std::string(pos - 1, '~');
        result += '^';
        result += std::string(len - pos, '~');
    }
    return result;
}

// std::string GetErrorMessagePointer(std::string &str, size_t seek, Position pos)
// {
//     const size_t threshold_length = 70;
//     std::string shown("  ");

//     if (str.length() > threshold_length) {
//         size_t e = (str.length() - seek < threshold_length / 2)
//                 ? str.length() - seek : threshold_length / 2;
//         shown += std::string(str.begin() + seek - threshold_length / 2,
//                         str.begin() + seek + e);
//         shown += "\n  " + CreateLLVMLikePointer(seek - threshold_length / 2,
//                                             threshold_length / 2 + e) + '\n';
//     } else {
//         shown += str + "\n  " + CreateLLVMLikePointer(pos.col_,
//             str.length()) + '\n';
//     }

//     return shown;
// }

Expression* Parser::ParseProgram()
{
    ExpressionList *exprs = builder()->NewExpressionList();
    try {
        while (peek() != END_OF_FILE) {
            exprs->Insert(ParseStatement());
        }
    } catch (std::exception &e) {
        auto error = dynamic_cast<SyntaxError*>(&e);

        if (error) {
            std::cerr << error->what() << " (" << error->token().position().row()
                    << ":" << error->token().position().col()
                    << ") (" << error->token().view() << ")" << std::endl;
        }
        throw;
    }

    return builder()->NewBlockStatement(exprs);
}

Expression *Parser::ParsePostfixExpression() {

    return nullptr;
}

Expression *ParseProgram(Parser *parser)
{
    // parse program and return AST
    return parser->ParseProgram();
}

}
