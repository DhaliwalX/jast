#include "parser/parser.h"
#include "parser/lexinfo.h"
#include "parser/ast-builder.h"
#include "parser/token.h"

namespace grok {
namespace parser {

bool IsAssign(TokenType tok)
{
    return tok >= ASSIGN && tok <= ASSIGN_MOD;
}

Parser::~Parser()
{
    delete builder_;
    delete lex_;
}

String Parser::GetStringLiteral()
{
    return LexerInfo::GetMatched();
}

double Parser::ParseNumber()
{
    String str = LexerInfo::GetMatched();
    try {
        double num = std::stod(str);
        return num;
    } catch(std::exception &e) {
        throw SyntaxError("invalid number (" + str + ")");
    }
}

String Parser::GetIdentifierName()
{
    return LexerInfo::GetMatched();
}

TokenType Parser::peek()
{
    return LexerInfo::peek();
}

void Parser::advance()
{
    LexerInfo::advance();
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
        if (tok != COMMA)
            throw SyntaxError("expected a ',' or ']'");
        advance();   
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
        } else if (tok == IDENTIFIER) {
            name = GetIdentifierName();
        } else {
            throw SyntaxError("expected an Identifier or a string");
        }

        advance();
        tok = peek();

        if (tok != COLON) {
            throw SyntaxError("expected a ':'");
        }
        advance();

        prop = ParseAssignExpression();
        proxy[name] = std::unique_ptr<Expression>(prop);

        // next token should be a ',' or '}'
        tok = peek();
        if (tok == RBRACE)
            break;
        if (tok != COMMA)
            throw SyntaxError("expected a ',' or '}'");
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
        result = builder()->NewIntegralLiteral(ParseNumber());
    } else if (tok == STRING) {
        result = builder()->NewStringLiteral(GetStringLiteral());
    } else if (tok == IDENTIFIER) {
        result = builder()->NewIdentifier(GetIdentifierName());
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
            throw SyntaxError("expected a ')'");
    } else if (tok == LBRACK) {
        result = ParseArrayLiteral();
    } else if (tok == LBRACE) {
        result = ParseObjectLiteral();
    } else if (tok == FUNCTION) {
        result = ParseFunctionStatement();
        return result;
    } else {
        throw SyntaxError("expected a primary expression");
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
    if (tok != IDENTIFIER)
        throw SyntaxError("expected a valid identifier");
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
    if (lex_->peek() != RBRACK)
        throw SyntaxError("expected a ']'");

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
    if (peek() != LBRACK && peek() != PERIOD)
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
    if (tok != LPAREN)
        throw SyntaxError("expected a '('");
    advance();

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
            throw SyntaxError("expected a ',' or ')'");
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

PrefixOperation MapTokenWithPrefixOperator(TokenType tok)
{
    switch(tok) {
        case INC:       return PrefixOperation::kIncrement;
        case DEC:       return PrefixOperation::kDecrement;
        case TYPEOF:    return PrefixOperation::kTypeOf;
        case DELETE:    return PrefixOperation::kDelete;
        case BIT_NOT:   return PrefixOperation::kBitNot;
        case NOT:       return PrefixOperation::kNot;
        case VOID:      return PrefixOperation::kVoid;
        default:    throw SyntaxError("unrecognised token");
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
            ParseExpression(), builder()->NewIntegralLiteral(1.0));
    } else if (tok == SUB) {
        advance();

        // similarly for `-Expr` to `Expr * -1` 
        return builder()->NewBinaryExpression(BinaryOperation::kMultiplication,
            ParseExpression(), builder()->NewIntegralLiteral(-1.0));
    } else if (tok == INC || tok == DEC || tok == NOT || tok == BIT_NOT
        || tok == TYPEOF || tok == DELETE || tok == VOID) {
        lex_->advance();

        return builder()->NewPrefixExpression(MapTokenWithPrefixOperator(tok),
            ParseLeftHandSideExpression());
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

BinaryOperation MapBinaryOperator(TokenType tok)
{
    switch(tok) {
        case ADD:           return BinaryOperation::kAddition;
        case SUB:           return BinaryOperation::kSubtraction;
        case MUL:           return BinaryOperation::kMultiplication;
        case DIV:           return BinaryOperation::kDivision;
        case MOD:           return BinaryOperation::kMod;
        case SHR:           return BinaryOperation::kShiftRight;
        case SAR:           return BinaryOperation::kShiftLeft;
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
        default:       throw SyntaxError("unexpected token as binary operator");
    }
}

/// reference for this function ::= llvm/examples/Kaleidoscope/chapter3/toy.cpp
/// if you are unable to understand the function just imagine you are 
/// parsing 2 + 3 * 5 - 6 / 7, (I too used that as a reference)
Expression* Parser::ParseBinaryExpressionRhs(int prec, Expression* lhs)
{
    while (true) {
        int tokprec = Token::precedance(peek());

        if (tokprec < prec) {
            return lhs;
        }

        // now we definitely have a binary operator
        auto tok = peek();
        advance();

        auto rhs = ParseUnaryExpression();

        auto nextprec = Token::precedance(peek());
        if (tokprec < nextprec) {
            rhs = ParseBinaryExpressionRhs(tokprec + 1, rhs);
        }

        // merge lhs and rhs
        lhs = builder()->NewBinaryExpression(MapBinaryOperator(tok), lhs, rhs);
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
        throw SyntaxError("expected a ':'");
    }

    // eat ':'
    advance();
    auto third = ParseAssignExpression();

    return builder()->NewTernaryExpression(first, second, third);
}

Expression* Parser::ParseCommaExpression()
{
    auto one = ParseAssignExpression();
    auto tok = lex_->peek();

    // if we have a comma ',', then we definitely have to parse
    // expr of type (expr, expr*)
    if (tok != COMMA)
        return one;

    ExpressionList *exprs = builder()->NewExpressionList();

    // loop until we don't find a ','
    while (true) {
        exprs->Insert(one);

        advance();
        one = ParseAssignExpression();

        tok = peek();
        if (tok != COMMA)
            break;   
    }

    return builder()->NewCommaExpression(exprs);
}

Expression* Parser::ParseExpressionOptional()
{
    auto tok = peek();

    if (tok == SEMICOLON) {
        return builder()->NewUndefinedLiteral();
    } else {
        return ParseCommaExpression();
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
    if (tok != LPAREN) 
        throw SyntaxError("expected a '('");
    advance();

    // parse the condition of if statement
    auto condition = ParseCommaExpression();

    tok = peek();
    if (tok != RPAREN)
        throw SyntaxError("expected a ')'");
    advance();

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

// TODO : Add parsing of for in statement
Expression* Parser::ParseForStatement()
{
    // eat 'for'
    advance();
    auto tok = peek();

    if (tok != LPAREN)
        throw SyntaxError("expected a '('");
    advance();

    // parse 'for ( >>this<< ;...' part
    auto init = ParseExpressionOptional();

    tok = peek();
    if (tok != SEMICOLON)
        throw SyntaxError("expected a ';'");
    advance();

    Expression* condition;
    if (peek() == SEMICOLON) {
        condition = builder()->NewBooleanLiteral(true);
    } else {
        // parse 'for (x = 10; >>this<< ...' part
        condition = ParseCommaExpression();
    }

    tok = peek();
    if (tok != SEMICOLON)
        throw SyntaxError("expected a ';'");
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
        throw SyntaxError("expected a ')'");
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
        throw SyntaxError("expected a '('");
    }
    advance();

    auto condition = ParseCommaExpression();
    tok = peek();
    if (tok != RPAREN)
        throw SyntaxError("expected a ')'");

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
        throw SyntaxError("expected 'while'");
    advance();

    tok = peek();
    if (tok != LPAREN) {
        throw SyntaxError("expected a '('");
    }
    advance();

    auto condition = ParseCommaExpression();
    tok = peek();
    if (tok != RPAREN) {
        throw SyntaxError("expected a ')'");
    }
    advance();

    tok = peek();
    if (tok != SEMICOLON)
        throw SyntaxError("expected a ';'");
    advance();

    return builder()->NewDoWhileStatement(condition, body);
}

std::vector<std::string> Parser::ParseParameterList()
{
    auto tok = peek();
    auto result = std::vector<std::string>();

    if (tok != LPAREN)
        throw SyntaxError("expected a '('");
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
            throw SyntaxError("expected an identifier");

        result.push_back(GetIdentifierName());
        advance();

        tok = peek();
        if (tok == RPAREN)
            break;

        if (tok != COMMA)
            throw SyntaxError("expected a ',' or ')'");
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
        auto stmt = ParseStatement();
        stmts->Insert(stmt);

        auto tok = peek();
        if (tok == RBRACE)
            break;
    }

    advance(); // eat '}'
    return builder()->NewBlockStatement(stmts);
}

Expression* Parser::ParseReturnStatement()
{
    advance(); // eat 'return'
    auto expr = ParseAssignExpression();
    auto tok = peek();

    if (tok != SEMICOLON)
        throw SyntaxError("expected a ';'");
    advance();
    return builder()->NewReturnStatement(expr);
}

Declaration* Parser::ParseDeclaration()
{
    auto tok = peek();
    if (tok != IDENTIFIER) {
        throw SyntaxError("expected an identifier");
    }
    std::string name = GetIdentifierName();
    advance();

    tok = peek();
    if (tok == SEMICOLON || tok == COMMA) {
        return builder()->factory()->NewDeclaration(
            builder()->locator()->loc(), name);
    } else if (tok != ASSIGN) {
        throw SyntaxError("expected a '='");
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
            throw SyntaxError("expected a ',' or ';'");
        advance(); // eat ','
    }

    advance(); // eat ';'
    return builder()->factory()->NewDeclarationList(
            builder()->locator()->loc(), std::move(decl_list));
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
            throw SyntaxError("expected a ';'");
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

Expression* Parser::ParseExpression()
{
    ExpressionList *exprs = builder()->NewExpressionList();
    try {
        while (peek() != EOS) {
            exprs->Insert(ParseStatement());
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }

    return builder()->NewBlockStatement(exprs);
}

}
}
