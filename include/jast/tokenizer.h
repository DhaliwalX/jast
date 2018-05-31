#ifndef LEXER_H_
#define LEXER_H_

#include "jast/tokens.h"
#include "jast/token.h"
#include "jast/scanner.h"
#include "jast/context.h"

namespace jast {

class TokenizerState;

/*
 * implementation of complete Tokenizer to be independant of flex
 */
class Tokenizer {
public:
    Tokenizer(CharacterStream *stream, ParserContext *context);
    ~Tokenizer();
    TokenType peek();

    void advance(bool divide_expected = false);

    void reset(CharacterStream *stream);

    Token &currentToken();

    ParserContext *context() { return context_; }

private:
    Token advance_internal(bool not_regex);
    Token parseString(char delim);
    Token parseNumber(char start);
    Token parseRegex(bool *ok);

    TokenizerState *state_;
    ParserContext *context_;
};

} // jast

#endif /* tokenizer.h included! */
