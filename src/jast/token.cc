#include "jast/token.h"
#include "jast/tokens.h"
#include <cassert>

namespace jast {
const int precedence_[NUM_TOKENS] = {
#define O(t, s, p) p,
#define K(t, s, p) p,
#define T(t, s, p) p,
#include "jast/tokens.inc"
};

const std::string strs_[NUM_TOKENS] = {
#define O(t, s, p) #s ,
#define K(t, s, p) #s ,
#define T(t, s, p) #s ,
#include "jast/tokens.inc"
};

Token::Token()
    : view_{ }
{ }

int Token::precedence(TokenType type) {
    assert(static_cast<int>(type) < NUM_TOKENS);

    return precedence_[static_cast<int>(type)];
}

std::string Token::str(TokenType type) {
    assert(static_cast<int>(type) < NUM_TOKENS);

    return strs_[static_cast<int>(type)];
}

}

bool IsKeyword(TokenType type) {
    switch (type) {
#define K(t, k, p) case t:
#include "jast/tokens.inc"
        return true;
    default:
        return false;
    }
}
