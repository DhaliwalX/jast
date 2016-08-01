#ifndef LEX_INFO_H_
#define LEX_INFO_H_

#include "parser/tokens.h"
#include "parser/string-builder.h"
#include <iostream>

namespace grok {
namespace parser {
class Scanner;

inline static void SetScanner(Scanner *scanner);

// simple scanner class
class Scanner {
public:
    Scanner(std::istream &is) : is_{ is } { }
    virtual ~Scanner() = default;

    // read a character from stream
    virtual char ReadChar();

private:
    std::istream &is_;
};

class LexerInfo {
    friend void SetScanner(Scanner *scanner);
public:

    // peeks the current token without moving forward
    static TokenType peek();

    // goes to next token
    static void advance();

    // advances and matches the current token with string
    static bool MatchNext(String str);

    // returns the current position
    static size_t GetSeek();

    // read a new character from Scanner
    static char ReadChar() { return scanner_->ReadChar(); }

    // returns the current matched string
    static String &GetMatched() { return current_matched_; }
private:
    static TokenType current_tok_;
    static String current_matched_;
    static size_t seek_;

    // pointer to scanner
    static Scanner *scanner_;
};

static void SetScanner(Scanner *scanner)
{
    LexerInfo::scanner_ = scanner;
}

}
}

#endif
