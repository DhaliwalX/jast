#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include "jast/tokens.h"
namespace jast {

// Position implementation
// -------------------------

class Position {
public:
    using size_type = std::string::size_type;

    Position()
    : col_ { 0 }, row_{ 0 }
    { }

    Position(size_type col, size_type row)
    : col_{ col }, row_ { row }
    { }

    size_type &col() { return col_; }
    size_type &row() { return row_; }

    const size_type &col() const { return col_; }
    const size_type &row() const { return row_; }
private:
    size_type col_;
    size_type row_;
};

class Token {
public:
    Token(std::string view, TokenType type, Position pos, size_t full_pos)
        : view_{ view }, type_{ type }, pos_{pos}, full_pos_{ full_pos }
    { }

    Token(int64_t num, TokenType type, Position pos, size_t full_pos)
        : num_{ num }, type_{ type }, pos_{pos}, full_pos_{ full_pos }
    { } 

    Token();

    TokenType &type() {
        return type_;
    }

    std::string &view() {
        return view_;
    }

    const std::string &view() const {
        return view_;
    }

    Position &position() {
        return pos_;
    }

    static int precedence(TokenType type);
    static std::string str(TokenType type);
private:
    std::string view_;
    int64_t num_;
    TokenType type_;
    Position pos_;
    size_t full_pos_;
};

}

#endif
