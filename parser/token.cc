#include "parser/token.h"
const char *Token::tokens_[NUM_TOKENS] = {
#define STR(t, s, p) s,
        TOKEN_LIST(STR)
#undef STR
};

const int Token::precedence_[NUM_TOKENS] = {
#define PRECEDENCE(t, s, p) p,
        TOKEN_LIST(PRECEDENCE)
#undef PRECEDENCE
};
