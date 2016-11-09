#include "parser/parser.h"
#include "parser/ast-builder.h"
#include "parser/astfactory.h"
#include "parser/lexinfo.h"
#include "parser/source-locator.h"
#include "parser/token.h"
#include "tests/common.h"

namespace {

#define GET_STATEMENT(ast, nr)  \
    *(ast->AsBlockStatement()->statements()->begin() + nr)

#define LOG_TEST_CASE(class, name, c, count)   \
    do { std::ostringstream os;  \
    std::string intro = "case #" + std::to_string(count);   \
    os << INTRO(class, name) << ": '" << c << "'";   \
    LOG(intro, os.str()); } while (0) 

#define LOG_TEST_CASE_PASSED(class, name, c, count)  \
    do { std::ostringstream os;  \
    std::string intro = "case #" + std::to_string(count++);   \
    os << INTRO(class, name) << ": '" << c << "' passed";   \
    LOG(intro, os.str());   } while(0)

#define TEST_INTEGRAL_INTERNAL(class, name, nr, count)   \
do {   \
    LOG_TEST_CASE(class, name, nr, count);  \
    input = #nr;    \
    input.push_back(';');   \
    std::unique_ptr<Expression> a{ ParseProgram(parser, input) };  \
    ASSERT_EQUAL(a->type(), ASTNodeType::kBlockStatement);    \
    Expression *ast = GET_STATEMENT(a, 0);    \
    ASSERT_EQUAL(ast->type(), ASTNodeType::kIntegralLiteral);   \
    ASSERT_EQUAL(ast->AsIntegralLiteral()->value(), nr);    \
    LOG_TEST_CASE_PASSED(class, name, nr, count);   \
} while (0)

#define TEST_INTEGRAL(nr)   \
    TEST_INTEGRAL_INTERNAL(SimpleSyntax, CheckIntegralLiteral, nr, count)

TEST(SimpleSyntax, CheckIntegralLiteral) {
    DECLARE_TEST(SimpleSyntax, CheckIntegralLiteral);
    TEST_SETUP(SimpleSyntax, CheckIntegralLiteral);

    {
        std::string input;
        int count = 1;

        TEST_INTEGRAL(10);
        TEST_INTEGRAL(10.0);
        TEST_INTEGRAL(0.10);
        TEST_INTEGRAL(0000);
        TEST_INTEGRAL(0);
        TEST_INTEGRAL(1e10);
        TEST_INTEGRAL(1.3e10);
        TEST_INTEGRAL(.10);
        TEST_INTEGRAL(0x12345678);
        TEST_INTEGRAL(0xaaaaaaa);
        TEST_INTEGRAL(0xabcdABCD);
        TEST_INTEGRAL(0xffffffff);
        TEST_INTEGRAL(0xdefDEF);
    }

    TEST_TEARDOWN(SimpleSyntax, CheckIntegralLiteral);
}

#define TEST_STRING_INTERNAL(class, name, c, count)  \
do {   \
    LOG_TEST_CASE(class, name, c, count);  \
    input = c;    \
    input.push_back(';');   \
    std::unique_ptr<Expression> a{ ParseProgram(parser, input) };  \
    ASSERT_EQUAL(a->type(), ASTNodeType::kBlockStatement);    \
    Expression *ast = GET_STATEMENT(a, 0);    \
    ASSERT_EQUAL(ast->type(), ASTNodeType::kStringLiteral);   \
    ASSERT_EQUAL(ast->AsStringLiteral()->string(), c);    \
    LOG_TEST_CASE_PASSED(class, name, c, count);   \
} while (0)

#define TEST_STRING(c)   \
    TEST_STRING_INTERNAL(SimpleSyntax, CheckStringLiteral, c, count)

TEST(SimpleSyntax, CheckStringLiteral) {
    DECLARE_TEST(SimpleSyntax, CheckStringLiteral);
    TEST_SETUP(SimpleSyntax, CheckStringLiteral);

    {
        std::string input;
        int count = 1;

        TEST_STRING("\"prince\"");
        TEST_STRING("\"prince'dhaliwal'\"");
        TEST_STRING("'prince \"dhaliwal\"'");
        TEST_STRING("\'\"\'\"'"); // this test fails
        TEST_STRING("\'101010\'");
    }

    TEST_TEARDOWN(SimpleSyntax, CheckStringLiteral);    
}

#undef TEST_STRING

TEST(SimpleSyntax, CheckNullLiteral) {
    DECLARE_TEST(SimpleSyntax, CheckNullLiteral);
    TEST_SETUP(SimpleSyntax, CheckNullLiteral);

    {
        std::string input;
        int count = 1;

        LOG_TEST_CASE(SimpleSyntax, CheckNullLiteral, "null", count);
        input = "null";
        input.push_back(';');
        std::unique_ptr<Expression> a{ ParseProgram(parser, input) };
        ASSERT_EQUAL(a->type(), ASTNodeType::kBlockStatement);
        Expression *ast = GET_STATEMENT(a, 0);
        ASSERT_EQUAL(ast->type(), ASTNodeType::kNullLiteral);
        LOG_TEST_CASE_PASSED(SimpleSyntax, CheckNullLiteral, input, count);        
    }

    TEST_TEARDOWN(SimpleSyntax, CheckNullLiteral);
}
TEST(SimpleSyntax, CheckUndefinedLiteral) {
    DECLARE_TEST(SimpleSyntax, CheckUndefinedLiteral);
    TEST_SETUP(SimpleSyntax, CheckUndefinedLiteral);

    {
        std::string input;
        int count = 1;

        LOG_TEST_CASE(SimpleSyntax, CheckUndefinedLiteral, "undefined", count);
        input = "undefined";
        input.push_back(';');
        std::unique_ptr<Expression> a{ ParseProgram(parser, input) };
        ASSERT_EQUAL(a->type(), ASTNodeType::kBlockStatement);
        Expression *ast = GET_STATEMENT(a, 0);
        ASSERT_EQUAL(ast->type(), ASTNodeType::kIdentifier);
        LOG_TEST_CASE_PASSED(SimpleSyntax, CheckUndefinedLiteral, input, count);
    }

    TEST_TEARDOWN(SimpleSyntax, CheckUndefinedLiteral);
}

#define TEST_BOOL_INTERNAL(class, name, c, count)  \
do {   \
    LOG_TEST_CASE(class, name, c, count);  \
    input = #c;    \
    input.push_back(';');   \
    std::unique_ptr<Expression> a{ ParseProgram(parser, input) };  \
    ASSERT_EQUAL(a->type(), ASTNodeType::kBlockStatement);    \
    Expression *ast = GET_STATEMENT(a, 0);    \
    ASSERT_EQUAL(ast->type(), ASTNodeType::kBooleanLiteral);   \
    ASSERT_EQUAL(ast->AsBooleanLiteral()->pred(), c);    \
    LOG_TEST_CASE_PASSED(class, name, c, count);   \
} while (0)

#define TEST_BOOL(c)   \
    TEST_BOOL_INTERNAL(SimpleSyntax, CheckBooleanLiteral, c, count)

TEST(SimpleSyntax, CheckBooleanLiteral) {
    DECLARE_TEST(SimpleSyntax, CheckStringLiteral);
    TEST_SETUP(SimpleSyntax, CheckStringLiteral);

    {
        std::string input;
        int count = 1;

        TEST_BOOL(false);
        TEST_BOOL(true);
    }

    TEST_TEARDOWN(SimpleSyntax, CheckStringLiteral);    
}

TEST(SimpleSyntax, CheckThisHolder) {
    DECLARE_TEST(SimpleSyntax, CheckThisHolder);
    TEST_SETUP(SimpleSyntax, CheckThisHolder);

    {
        std::string input;
        int count = 1;

        LOG_TEST_CASE(SimpleSyntax, CheckThisHolder, "this", count);
        input = "this";
        input.push_back(';');
        std::unique_ptr<Expression> a{ ParseProgram(parser, input) };
        ASSERT_EQUAL(a->type(), ASTNodeType::kBlockStatement);
        Expression *ast = GET_STATEMENT(a, 0);
        ASSERT_EQUAL(ast->type(), ASTNodeType::kThisHolder);
        LOG_TEST_CASE_PASSED(SimpleSyntax, CheckThisHolder, input, count);
    }

    TEST_TEARDOWN(SimpleSyntax, CheckUndefinedLiteral);
}

#define TEST_IDENTIFIER(id) \
    do {   \
        LOG_TEST_CASE(SimpleSyntax, CheckIdentifier, id, count);  \
        input = id; \
        input.push_back(';');   \
        std::unique_ptr<Expression> a{ ParseProgram(parser, input) };   \
        ASSERT_EQUAL(a->type(), ASTNodeType::kBlockStatement);  \
        Expression *ast = GET_STATEMENT(a, 0);  \
        ASSERT_EQUAL(ast->type(), ASTNodeType::kIdentifier);    \
        ASSERT_EQUAL(ast->AsIdentifier()->GetName(), id);   \
        LOG_TEST_CASE_PASSED(SimpleSyntax, CheckIdentifier, id, count);\
    } while (0)

TEST(SimpleSyntax, CheckIdentifier) {
    DECLARE_TEST(SimpleSyntax, CheckIdentifier);
    TEST_SETUP(SimpleSyntax, CheckIdentifier);

    {
        std::string input;
        int count = 1;

        TEST_IDENTIFIER("prince");
        TEST_IDENTIFIER("_dhaliwal");
        TEST_IDENTIFIER("_0");
        TEST_IDENTIFIER("_");
        TEST_IDENTIFIER("_prince_1924023_dhaliwal");
        TEST_IDENTIFIER("$");
        TEST_IDENTIFIER("$$");
        TEST_IDENTIFIER("$prince");
        TEST_IDENTIFIER("$prince123_$");
        TEST_IDENTIFIER("$_");
    }

    TEST_TEARDOWN(SimpleSyntax, CheckUndefinedLiteral);
}

// TEST(SimpleSyntax, CheckArgumentList) {

// }

}

int main()
{
    CheckIntegralLiteral();
    CheckStringLiteral();
    CheckNullLiteral();
    CheckUndefinedLiteral();
    CheckBooleanLiteral();
    CheckThisHolder();
    CheckIdentifier();
    return 0;
}
