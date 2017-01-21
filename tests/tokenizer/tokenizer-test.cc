#include <jast/scanner.h>
#include <jast/tokenizer.h>

#include <gtest/gtest.h>

#include <cassert>
#include <string>
#include <sstream>

using namespace jast;

#define INIT(s)  \
    std::istringstream str(s);   \
    Scanner scanner = Scanner(str); \
    Tokenizer tokenizer(&scanner)

#define TOKENIZER_TEST(str, expected, eq) \
do {    \
    INIT(str);   \
    ASSERT_EQ(tokenizer.peek(), TokenType::expected); \
    ASSERT_EQ(tokenizer.currentToken().view(), eq); \
} while (0)

namespace {
class TokenizerTest : public ::testing::Test {
public:
    void SetUp() override {}
};


TEST_F(TokenizerTest, Identifier) {
    TOKENIZER_TEST("abc", IDENTIFIER, "abc");
    TOKENIZER_TEST("_", IDENTIFIER, "_");
    TOKENIZER_TEST("$", IDENTIFIER, "$");
    TOKENIZER_TEST("$_", IDENTIFIER, "$_");
    TOKENIZER_TEST("$123", IDENTIFIER, "$123");
    TOKENIZER_TEST("$_23_234_sv", IDENTIFIER, "$_23_234_sv");
}

TEST_F(TokenizerTest, Integers) {
    TOKENIZER_TEST("123", NUMBER, "123");
    TOKENIZER_TEST("0", NUMBER, "0");
    TOKENIZER_TEST("2", NUMBER, "2");
}

TEST_F(TokenizerTest, RealNumbers) {
    TOKENIZER_TEST("1.2", NUMBER, "1.2");
    TOKENIZER_TEST("1.23.3.2", NUMBER, "1.23");
    TOKENIZER_TEST("1.23", NUMBER, "1.23");
    TOKENIZER_TEST("0.2343432", NUMBER, "0.2343432");
    TOKENIZER_TEST(".23432", NUMBER, ".23432");
    TOKENIZER_TEST("1e10", NUMBER, "1e10");
    TOKENIZER_TEST("1.2e.123", NUMBER, "1.2e");
    TOKENIZER_TEST("1.23e103", NUMBER, "1.23e103");
}

TEST_F(TokenizerTest, HexNumbers) {
    TOKENIZER_TEST("0x123", NUMBER, "0x123");
    TOKENIZER_TEST("0x1234567890abcdef", NUMBER, "0x1234567890abcdef");
    TOKENIZER_TEST("0xabcdef", NUMBER, "0xabcdef");
    TOKENIZER_TEST("0xa", NUMBER, "0xa");
    TOKENIZER_TEST("0XABC", NUMBER, "0XABC");
}

TEST_F(TokenizerTest, OctalNumbers) {
    TOKENIZER_TEST("01234", NUMBER, "01234");
}

#define O(t, k, p) \
    TOKENIZER_TEST(k, t, k);

TEST_F(TokenizerTest, Operators) {
#include "jast/tokens.inc"
}

#define K(t, k, p)  \
    TOKENIZER_TEST(k, t, k);

TEST_F(TokenizerTest, Keywords) {
#include "jast/tokens.inc"
}

TEST_F(TokenizerTest, Strings) {
    TOKENIZER_TEST("\"Jast fsd34e093-=432;';\"", STRING, "Jast fsd34e093-=432;';");
    TOKENIZER_TEST("\"\"", STRING, "");
    TOKENIZER_TEST("''", STRING, "");
    TOKENIZER_TEST("'\"'", STRING, "\"");
}

TEST_F(TokenizerTest, ComplexTest1) {
    INIT("jast 1.23");
    ASSERT_EQ(tokenizer.peek(), TokenType::IDENTIFIER);
    tokenizer.advance();
    ASSERT_EQ(tokenizer.peek(), TokenType::NUMBER);
}

#define CHECK(tok)  \
    ASSERT_EQ(tokenizer.peek(), tok);   \
    tokenizer.advance()

TEST_F(TokenizerTest, ComplexTest2) {
    INIT("1.23.poer??:-=+");
    CHECK(NUMBER);
    CHECK(PERIOD);
    CHECK(IDENTIFIER);
    CHECK(CONDITIONAL);
    CHECK(CONDITIONAL);
    CHECK(COLON);
    CHECK(ASSIGN_SUB);
}

TEST_F(TokenizerTest, ComplexTest3) {
    INIT("++--+-=+__$()**+++");
    CHECK(INC);
    CHECK(DEC);
    CHECK(ADD);
    CHECK(ASSIGN_SUB);
    CHECK(ADD);
    CHECK(IDENTIFIER);
    CHECK(LPAREN);
    CHECK(RPAREN);
    CHECK(MUL);
    CHECK(MUL);
    CHECK(INC);
    CHECK(ADD);
}

TEST_F(TokenizerTest, ComplextTest4) {
    INIT("Infinity in window & !!0 == !!!1 & +~~_ + \"\" === _ ? _ + _x : _ + _y");

    CHECK(IDENTIFIER);
    CHECK(IN);
    CHECK(IDENTIFIER);
    CHECK(BIT_AND);
    CHECK(NOT);
    CHECK(NOT);
    CHECK(NUMBER);
    CHECK(EQ);
    CHECK(NOT);
    CHECK(NOT);
    CHECK(NOT);
    CHECK(NUMBER);
    CHECK(BIT_AND);
    CHECK(ADD);
    CHECK(BIT_NOT);
    CHECK(BIT_NOT);
    CHECK(IDENTIFIER);
    CHECK(ADD);
    CHECK(STRING);
    CHECK(EQ_STRICT);
    CHECK(IDENTIFIER);
    CHECK(CONDITIONAL);
    CHECK(IDENTIFIER);
    CHECK(ADD);
    CHECK(IDENTIFIER);
    CHECK(COLON);
    CHECK(IDENTIFIER);
    CHECK(ADD);
    CHECK(IDENTIFIER);
}

TEST_F(TokenizerTest, ComplexTest5) {
    INIT(";\na=b");

    CHECK(SEMICOLON);
    CHECK(IDENTIFIER);
    CHECK(ASSIGN);
    CHECK(IDENTIFIER);
}

TEST_F(TokenizerTest, ComplexTest6) {
    INIT(";\na=b;\na;");

    CHECK(SEMICOLON);
    CHECK(IDENTIFIER);
    CHECK(ASSIGN);
    CHECK(IDENTIFIER);
    CHECK(SEMICOLON);
    CHECK(IDENTIFIER);
    CHECK(SEMICOLON);
}

TEST_F(TokenizerTest, ComplexTest7) {
    INIT("{a+=10;\na=b;\n}\n");

    CHECK(LBRACE);
    CHECK(IDENTIFIER);
    CHECK(ASSIGN_ADD);
    CHECK(NUMBER);
    CHECK(SEMICOLON);
    CHECK(IDENTIFIER);
    CHECK(ASSIGN);
    CHECK(IDENTIFIER);
    CHECK(SEMICOLON);
    CHECK(RBRACE);
}

TEST_F(TokenizerTest, ComplexTest8) {
    INIT("a < 10;\n");

    CHECK(IDENTIFIER);
    CHECK(LT);
    CHECK(NUMBER);
    CHECK(SEMICOLON);
}

}
