#ifndef TOKENS_H_
#define TOKENS_H_

#define TOKEN_LIST(T)                                                  \
    T(EOS, "EOS", 0)                                                   \
    T(LPAREN, "(", 0)                                                  \
    T(RPAREN, ")", 0)                                                  \
    T(LBRACK, "[", 0)                                                  \
    T(RBRACK, "]", 0)                                                  \
    T(LBRACE, "{", 0)                                                  \
    T(RBRACE, "}", 0)                                                  \
    T(COLON, ":", 0)                                                   \
    T(SEMICOLON, ";", 0)                                               \
    T(PERIOD, ".", 0)                                                  \
    T(ELLIPSIS, "...", 0)                                              \
    T(CONDITIONAL, "?", 2)                                             \
    T(INC, "++", 0)                                                    \
    T(DEC, "--", 0)                                                    \
    T(ARROW, "=>", 0)                                                  \
    T(ASSIGN, "=", 2)                                                  \
    T(ASSIGN_BIT_OR, "|=", 2)                                          \
    T(ASSIGN_BIT_XOR, "^=", 2)                                         \
    T(ASSIGN_BIT_AND, "&=", 2)                                         \
    T(ASSIGN_SHL, "<<=", 2)                                            \
    T(ASSIGN_SAR, ">>=", 2)                                            \
    T(ASSIGN_SHR, ">>>=", 2)                                           \
    T(ASSIGN_ADD, "+=", 2)                                             \
    T(ASSIGN_SUB, "-=", 2)                                             \
    T(ASSIGN_MUL, "*=", 2)                                             \
    T(ASSIGN_DIV, "/=", 2)                                             \
    T(ASSIGN_MOD, "%=", 2)                                             \
    T(COMMA, ",", 1)                                                   \
    T(OR, "||", 4)                                                     \
    T(AND, "&&", 5)                                                    \
    T(BIT_OR, "|", 6)                                                  \
    T(BIT_XOR, "^", 7)                                                 \
    T(BIT_AND, "&", 8)                                                 \
    T(SHL, "<<", 11)                                                   \
    T(SAR, ">>", 11)                                                   \
    T(SHR, ">>>", 11)                                                  \
    T(ADD, "+", 12)                                                    \
    T(SUB, "-", 12)                                                    \
    T(MUL, "*", 13)                                                    \
    T(DIV, "/", 13)                                                    \
    T(MOD, "%", 13)                                                    \
    T(EQ, "==", 9)                                                     \
    T(NE, "!=", 9)                                                     \
    T(EQ_STRICT, "===", 9)                                             \
    T(NE_STRICT, "!==", 9)                                             \
    T(LT, "<", 10)                                                     \
    T(GT, ">", 10)                                                     \
    T(LTE, "<=", 10)                                                   \
    T(GTE, ">=", 10)                                                   \
    T(INSTANCEOF, "instanceof", 10)                                    \
    T(IN, "in", 10)                                                    \
    T(NOT, "!", 0)                                                     \
    T(BIT_NOT, "~", 0)                                                 \
    T(DELETE, "delete", 0)                                             \
    T(TYPEOF, "typeof", 0)                                             \
    T(VOID, "void", 0)                                                 \
    T(BREAK, "break", 0)                                               \
    T(CASE, "case", 0)                                                 \
    T(CATCH, "catch", 0)                                               \
    T(CONTINUE, "continue", 0)                                         \
    T(DEBUGGER, "debugger", 0)                                         \
    T(DEFAULT, "default", 0)                                           \
    T(DO, "do", 0)                                                     \
    T(ELSE, "else", 0)                                                 \
    T(FINALLY, "finally", 0)                                           \
    T(FOR, "for", 0)                                                   \
    T(FUNCTION, "function", 0)                                         \
    T(IF, "if", 0)                                                     \
    T(NEW, "new", 0)                                                   \
    T(RETURN, "return", 0)                                             \
    T(SWITCH, "switch", 0)                                             \
    T(THIS, "this", 0)                                                 \
    T(THROW, "throw", 0)                                               \
    T(TRY, "try", 0)                                                   \
    T(VAR, "var", 0)                                                   \
    T(WHILE, "while", 0)                                               \
    T(WITH, "with", 0)                                                 \
    T(NULL_LITERAL, "null", 0)                                         \
    T(TRUE_LITERAL, "true", 0)                                         \
    T(FALSE_LITERAL, "false", 0)                                       \
    T(ASYNC, "async", 0)                                               \
    T(AWAIT, "await", 0)                                               \
    T(CLASS, "class", 0)                                               \
    T(CONST, "const", 0)                                               \
    T(ENUM, "enum", 0)                                                 \
    T(EXPORT, "export", 0)                                             \
    T(EXTENDS, "extends", 0)                                           \
    T(IMPORT, "import", 0)                                             \
    T(LET, "let", 0)                                                   \
    T(STATIC, "static", 0)                                             \
    T(YIELD, "yield", 0)                                               \
    T(SUPER, "super", 0)                                               \
    T(NUMBER, "NUMBER", 0)                                             \
    T(STRING, "STRING", 0)                                             \
    T(IDENTIFIER, "ID", 0)                                             \
    T(ILLEGAL, "ILLEGAL", 0)


enum TokenType {
#define TYPE(t, s, p) t,
    TOKEN_LIST(TYPE)
#undef TOKEN_STR
    NUM_TOKENS
};

#endif
