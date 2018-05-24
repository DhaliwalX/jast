#ifndef LEXER_H_
#define LEXER_H_

#include "jast/tokens.h"
#include "jast/token.h"
#include "jast/scanner.h"

namespace jast {

class TokenizerState;

/*
 * implementation of complete Tokenizer to be independant of flex
 */
class Tokenizer {
public:
    Tokenizer(CharacterStream *stream);
    TokenType peek();

    void advance();

    void reset(CharacterStream *stream);

    Token &currentToken();

private:
    Token advance_internal();
    Token parseString(char delim);
    Token parseNumber(char start);
    Token parseRegex(bool *ok);

    TokenizerState *state_;
};

} // jast

#endif /* tokenizer.h included! */
