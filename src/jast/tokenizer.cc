#include "jast/tokenizer.h"
#include "jast/token.h"
#include "jast/scanner.h"

#include <cstring>
#include <cassert>
#include <cctype>
#include <iostream>
#include <unordered_map>

namespace jast {


// TokenizerState implementation
// -------------------------------

class TokenizerState {
public:
    static std::unordered_map<std::string, TokenType> keywords;
    friend class Tokenizer;

    using size_type = size_t;
    using seek_type = size_t;
    using char_type = char;

    TokenizerState(CharacterStream *scanner)
    : seek_{ 0 }, scanner_{ scanner }
    { }

    TokenizerState(const TokenizerState &state) = default;
    TokenizerState(TokenizerState &&state) = default;

    inline void reset(CharacterStream *scanner) {
        scanner_ = scanner;
        seek_ = 0;
        last_col_length_ = 0;
        position_ = Position();
    }

    inline size_type &seek() {
        return seek_;
    }

    inline Position &position() {
        return position_;
    }

    inline Token &token() {
        return token_;
    }

    inline Token &last_token() {
        return last_token_;
    }

    inline void setToken(Token token) {
        last_token_ = token_;
        token_ = token;
    }

    inline void setPosition(int col, int row) {
        position_.col() = col;
        position_.row() = row;
    }

    inline const Position &position() const {
        return position_;
    }

    inline char_type readchar() {
        char ch = scanner_->readchar();
        if (ch == '\n') {
            position_.row()++;

            // save the last column position
            last_col_length_ = position_.col();
            position_.col() = 0;
        } else if (ch == EOF) {
            // in case of EOF we don't want to go outside the limit
            // of our source code
            return EOF;
        } else {
            position_.col()++;
        }

        seek_++;
        return ch;
    }

    inline void putback(char_type ch) {
        seek_--;
        if (ch == EOF)
            return;
        if (ch == '\n') {
            assert(position_.row() != 0);
            // FIXME: we can only go back to last line correctly
            position_.col() = last_col_length_;
            position_.row()--;
        } else {
            assert(position_.col() != 0);
            position_.col()--;
        }
        scanner_->putback(ch);
    }

private:
    seek_type seek_;

    Token token_;
    Token last_token_;

    Position position_;
    size_t last_col_length_;
    CharacterStream *scanner_;
};

std::unordered_map<std::string, TokenType> TokenizerState::keywords = {
#define K(t, k, p) { k, t },
#include "jast/tokens.inc"
};

// small utility functions
bool isValidIdentifierStart(char ch) {
    return std::isalpha(ch) || ch == '_' || ch == '$';
}

bool isValidIdentifierChar(char ch) {
    return std::isalnum(ch) || ch == '_' || ch == '$';
}

TokenType isKeyword(const std::string &str) {
    auto it = TokenizerState::keywords.find(str);
    if (it == TokenizerState::keywords.end())
        return TokenType::IDENTIFIER;
    return it->second;
}

TokenType isOneCharacterSymbol(char ch) {
    // returns the type of token from the given symbol
  switch (ch) {
  case '(':
    return LPAREN;
  case ')':
    return RPAREN;
  case '{':
    return LBRACE;
  case '}':
    return RBRACE;
  case '[':
    return LBRACK;
  case ']':
    return RBRACK;
  case ':':
    return COLON;
  case ';':
    return SEMICOLON;
  case '.':
    return PERIOD;
  case ',':
    return COMMA;
  case '?':
    return CONDITIONAL;
  case '+':
    return ADD;
  case '-':
    return SUB;
  case '/':
    return DIV;
  case '*':
    return MUL;
  case '%':
    return MOD;
  case '>':
    return GT;
  case '<':
    return LT;
  case '=':
    return ASSIGN;
  case '^':
    return BIT_XOR;
  case '|':
    return BIT_OR;
  case '&':
    return BIT_AND;
  case '!':
    return NOT;
  case '~':
    return BIT_NOT;
  default:
    return INVALID;
  }
}

TokenType isTwoCharacterSymbol(char ch1, char ch2) {
// returns the type of symbol of two characters
    switch (ch2) {
    case '=':
        switch (ch1) {
        case '+':
            return ASSIGN_ADD;
        case '-':
            return ASSIGN_SUB;
        case '*':
            return ASSIGN_MUL;
        case '/':
            return ASSIGN_DIV;
        case '%':
            return ASSIGN_MOD;
        case '^':
            return ASSIGN_BIT_XOR;
        case '&':
            return ASSIGN_BIT_AND;
        case '|':
            return ASSIGN_BIT_OR;
        case '>':
            return GTE;
        case '<':
            return LTE;
        case '=':
            return EQ;
        case '!':
            return NE;
        }
        return INVALID;
    case '|':
        if (ch1 == '|')
            return OR;
        return INVALID;
    case '&':
        if (ch1 == '&')
            return AND;
        return INVALID;
    case '+':
        if (ch1 == '+')
            return INC;
        return INVALID;
    case '-':
        if (ch1 == '-')
            return DEC;
        return INVALID;
    case '<':
        if (ch1 == '<')
            return SHL;
        return INVALID;
    case '>':
        if (ch1 == '>')
            return SAR;
    else if (ch1 == '=')
        return ARROW;
    default:
        return INVALID;
    }
}

TokenType isThreeCharacterSymbol(char ch1, char ch2, char ch3) {
    if (ch1 == '=' && ch2 == '=' && ch3 == '=')
        return TokenType::EQ_STRICT;
    else if (ch1 == '!' && ch2 == '=' && ch3 == '=')
        return TokenType::NE_STRICT;
    else if (ch1 == '>' && ch2 == '>' && ch3 == '>')
        return TokenType::SHR;
    else if (ch1 == '>' && ch2 == '>' && ch3 == '=')
        return TokenType::ASSIGN_SAR;
    else if (ch1 == '<' && ch2 == '<' && ch3 == '=')
        return TokenType::ASSIGN_SHL;
    else if (ch1 == '.' && ch2 == '.' && ch3 == '.')
        return TokenType::ELLIPSIS;
    return INVALID;
}

bool IsSpace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
}


// Tokenizer implementation
// --------------------------

Tokenizer::Tokenizer(CharacterStream *stream)
    : state_{ new TokenizerState(stream) }
{ }

#define _ state_->

void Tokenizer::reset(CharacterStream *stream) {
    state_->reset(stream);
}

void Tokenizer::advance() {
    _ setToken(advance_internal());
}

TokenType Tokenizer::peek() {
    if (_ seek() == 0)
        advance();
    return _ token().type();
}

Token &Tokenizer::currentToken() {
    return _ token();
}

// The heart of the lexer
// -----------------------
Token Tokenizer::advance_internal() {
    char ch = _ readchar();

    do {
        // skip all non printable characters
        while (ch != EOF && IsSpace(ch))
            ch = _ readchar();


        if (ch == '/') {
            char next = _ readchar();

            if (next == EOF) {
                return Token(std::string(1, ch), TokenType::DIV, _ position(),
                        _ seek());
            } else if (next == '/') {
                // single line comment skip whole line
                while (ch != '\n') {
                    ch = _ readchar();
                }
            } else if (next == '*') {
                // block comment
                ch = _ readchar();
                char last = ch;

                while (ch != EOF && ch != '/' && last != '*') {
                    last = ch;
                    ch = _ readchar();
                }

                if (ch == EOF) {
                    return Token(std::string("ERROR"), TokenType::ERROR,
                        _ position(), _ seek());
                }
            } else {
                bool ok = true;
                _ putback(next);
                Token t = parseRegex(&ok);
                if (!ok) {
                    break;
                } else {
                    return t;
                }
            }
        } else if (!IsSpace(ch)) {
            break;
        } else if (ch == EOF) {
            return Token(std::string("EOF"), TokenType::END_OF_FILE,
                _ position(), _ seek());
        }
    } while (true);
    auto seek = _ seek();
    auto position = _ position();

    // now check the various possibilities of tokens
    // Possibility 1:
    //  token is a valid identifier, or keyword

    if (isValidIdentifierStart(ch)) {
        std::string identifier;
        identifier += ch;

        ch = _ readchar();
        while (isValidIdentifierChar(ch)) {
            identifier += ch;
            ch = _ readchar();
        }

        _ putback(ch);

        return Token(identifier, isKeyword(identifier), position, seek);
    }

    if (ch == EOF) {
        return Token(std::string("EOF"), TokenType::END_OF_FILE,
            position, seek);
    }

    char first = ch;
    char second = _ readchar();
    char third = _ readchar();

    TokenType type;
    std::string view;

    if (first != EOF && second != EOF) {
        type = isThreeCharacterSymbol(first, second, third);
        if (type == TokenType::SHR) {
            char fourth = _ readchar();

            if (fourth == '=') {
                // >>>=
                view = view + first + second + third + fourth;
                return Token(view, ASSIGN_SHR, _ position(), _ seek());
            } else {
                _ putback(fourth);
                view = view + first + second + third;
                return Token(view, type, position, seek);
            }
        } else if (type != INVALID) {
            view = view + first + second + third;
            return Token(view, type, position, seek);
        }
        _ putback(third);
    }



    if (second != EOF) { 
        type = isTwoCharacterSymbol(first, second);

        if (type != INVALID) {
            view = view + first + second;
            return Token(view, type, position, seek);
        }
        _ putback(second);
    }


    type = isOneCharacterSymbol(first);

    if (type == TokenType::PERIOD) {
        if (isdigit(second)) {
            return parseNumber(first);
        }
    }
    if (type != INVALID) {
        view += first;
        return Token(view, type, position, seek);
    }

    if (ch == '"' || ch == '\'' || ch == '`') {
        return parseString(ch);
    }

    if (isdigit(ch)) {
        return parseNumber(ch);
    } else if (ch == EOF) {
        return Token(std::string("EOF"), TokenType::END_OF_FILE, position, seek);
    }

    return Token(std::string("ILLEGAL"), TokenType::INVALID, position, seek);
}

Token Tokenizer::parseRegex(bool *ok) {
    std::string buffer;
    auto seek = _ seek();
    auto position = _ position();

    char ch = _ readchar();

    while (ch != '/') {
        if (ch == EOF || ch == '\n') {
            *ok = false;
            if (ch == '\n') {
                _ putback('\n');
            }
            for (int i = buffer.length() - 1; i >= 0; i--) {
                _ putback(buffer[i]);
            }
            return Token(std::string("ERROR"), TokenType::ERROR, position, seek);
        }

        if (ch == '\\') {
            buffer.push_back(ch);
            ch = _ readchar();
        }

        buffer.push_back(ch);
        ch = _ readchar();
    }

    // adding end marker
    buffer.push_back('$');

    // parse regex flags g, i, m, u, y
    // Including special case n when there is no flag present
    ch = _ readchar();
    bool flag_present = false;
    while (ch == 'g' || ch == 'i' || ch == 'm' || ch == 'u' || ch == 'y') {
        flag_present = true;
        buffer.push_back(ch);
        ch = _ readchar();
    }

    _ putback(ch);

    if (!flag_present) {
        buffer.push_back('n');
    }

    return Token(buffer, TokenType::REGEX, position, seek);
}

Token Tokenizer::parseString(char delim) {
    std::string buffer;

    auto seek = _ seek();
    auto position = _ position();
    char ch = _ readchar();
    while (ch != EOF && ch != delim) {
        // escape characters
        if (ch == '\\') {
            buffer.push_back(ch);
            ch = _ readchar();
            if (ch == EOF) {
                break;
            }
        }
        buffer.push_back(ch);
        ch = _ readchar();
    }

    if (ch == EOF) {
        return Token(std::string("EOF"), TokenType::ERROR, position, seek);
    }

    TokenType type = delim == '`' ? TokenType::TEMPLATE : TokenType::STRING;

    return Token(buffer, type, position, seek);
}

Token Tokenizer::parseNumber(char start) {
    std::string buffer;
    auto seek = _ seek();
    bool had_exp = false;
    bool seen_dot = false;
    bool isdouble = false;
    auto position = _ position();
    char ch = _ readchar();

    buffer.push_back(start);

    if (tolower(ch) == 'x') {
        // parsing hex number
        buffer.push_back(ch);
        ch = _ readchar();
        while (ch != EOF && (tolower(ch) == 'a' || tolower(ch) == 'b'
            || tolower(ch) == 'c' || tolower(ch) == 'd'
            || tolower(ch) == 'e' || tolower(ch) == 'f'
            || isdigit(ch))) {
            buffer.push_back(ch);
            ch = _ readchar();
        }

    } else if (tolower(ch) == 'b') {
        // parsing a bin number
        buffer.push_back(ch);
        ch = _ readchar();
        while (ch != EOF && (ch == '0' || ch == '1')) {
            buffer.push_back(ch);
            ch = _ readchar();
        }

    } else if (tolower(ch) == 'o') {
        // parsing oct number
        while (ch != EOF && ch < '8' && ch >= '0') {
            buffer.push_back(ch);
            ch = _ readchar();
        }

    } else {
        if (start == '.' || ch == '.') {
            seen_dot = true;
            isdouble = true;
        }

        if (ch != EOF)
            buffer.push_back(ch);

        while (isdigit(ch) || (tolower(ch) == 'e') || ch == '.') {
            ch = _ readchar();

            if (ch == '.') {
                if (!seen_dot) {
                    seen_dot = true;
                    isdouble = true;
                    buffer.push_back(ch);
                } else {
                    // not the character that we should care about
                    break;
                }
            } else if (tolower(ch) == 'e') {
                if (!had_exp) {
                    had_exp = true;
                    buffer.push_back(ch);
                    isdouble = true;
                } else {
                    break;
                }
            } else if (isdigit(ch)) {
                buffer.push_back(ch);
            } else if (ch == EOF) {
                if (start == '.' && buffer.length() == 1) {
                    return Token(std::string("."), TokenType::PERIOD, position,
                        seek);
                }
                break;
            } else {
                break;
            }
        }
    }


    _ putback(ch);

    if (!isdouble) {
        return Token(buffer, TokenType::NUMBER, position, seek);
    } else {
        return Token(buffer, TokenType::NUMBER, position, seek);
    }
}

} // jast

