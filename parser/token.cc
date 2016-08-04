#include "parser/token.h"
const char *tokens_[NUM_TOKENS] = {
#define STR(t, s, p) s,
        TOKEN_LIST(STR)
#undef STR
};

const int precedence_[NUM_TOKENS] = {
#define PRECEDENCE(t, s, p) p,
        TOKEN_LIST(PRECEDENCE)
#undef PRECEDENCE
};
