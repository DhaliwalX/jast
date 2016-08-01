#ifndef TOKEN_H_
#define TOKEN_H_

#include "parser/tokens.h"
#include <string>

class Token {
public:
    static int precedance(int tok) { return precedence_[tok]; }

    static const char *str(int tok) { return tokens_[tok]; }
private:
    static const char *tokens_[NUM_TOKENS];
    static const int precedence_[NUM_TOKENS];
};

#endif
