#include "parser/parser.h"
#include "parser/ast-builder.h"
#include "parser/astfactory.h"
#include "parser/lexinfo.h"
#include "parser/source-locator.h"
#include "parser/token.h"

#include <iostream>
#include <cassert>
#include <map>
#include <vector>

typedef void(*test_function_t)(void);
std::map<std::string, std::vector<test_function_t>> tests;

#define INTRO(class, name) \
    std::string(#class) + "." + #name

#define ENTER_TEST_STRING(class, test_name)  \
    std::string("Entering test: ") + #class + "." + #test_name

#define END_TEST_STRING(class, name)    \
    std::string("ran ") + #class + "." + #name

#define PASSED_TEST_STRING(class, name) \
    std::string("passed ") + #class + "." + #name

#define LOG(prefix, msg)  \
    std::cerr << "[ " << prefix << " ] " << msg << std::endl

#define LOG_START(class, name)    \
    LOG("START", ENTER_TEST_STRING(class, name))

#define LOG_END(class, name)  \
    LOG("END", END_TEST_STRING(class, name))

#define LOG_SUCCESS(class, name)    \
    LOG("PASSED", PASSED_TEST_STRING(class, name)) 

#define ASSERT_EQUAL(a, b)  \
    assert((a == b))

#define ASSERT_NOT_EQUAL(a, b)  \
    assert(!(a == b))

#define DECLARE_TEST(class, name)

#define TEST_SETUP(class, test_name)   \
    using namespace grok::parser;   \
    ParserContext ctx{};    \
    LexerInfo lex;  \
    SourceLocator locator(&lex);    \
    ASTFactory *factory = ASTFactory::GetFactoryInstance(); \
    ASTBuilder *builder = new ASTBuilder(&ctx, factory, &locator);  \
    Parser *parser = new Parser(&ctx, builder, &lex);   \
    LOG_START(class, test_name)

#define TEST_TEARDOWN(class, test_name)    \
    delete parser;  \
    delete builder; \
    LOG_END(class, test_name)

#define TEST(class, name)   \
    void name()

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
    TEST_INTEGRAL_INTERNAL(Syntax, CheckIntegralLiteral, nr, count)

TEST(Syntax, CheckIntegralLiteral) {
    DECLARE_TEST(Syntax, CheckIntegralLiteral);
    TEST_SETUP(Syntax, CheckIntegralLiteral);

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

    TEST_TEARDOWN(Syntax, CheckIntegralLiteral);
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
    TEST_STRING_INTERNAL(Syntax, CheckStringLiteral, c, count)

TEST(Syntax, CheckStringLiteral) {
    DECLARE_TEST(Syntax, CheckStringLiteral);
    TEST_SETUP(Syntax, CheckStringLiteral);

    {
        std::string input;
        int count = 1;

        TEST_STRING("\"prince\"");
        TEST_STRING("\"prince'dhaliwal'\"");
        TEST_STRING("'prince \"dhaliwal\"'");
        TEST_STRING("\'\"\'\"'"); // this test fails
        TEST_STRING("\'101010\'");
    }

    TEST_TEARDOWN(Syntax, CheckStringLiteral);    
}

#undef TEST_STRING

TEST(Syntax, CheckNullLiteral) {
    DECLARE_TEST(Syntax, CheckNullLiteral);
    TEST_SETUP(Syntax, CheckNullLiteral);

    {
        std::string input;
        int count = 1;

        LOG_TEST_CASE(Syntax, CheckNullLiteral, "null", count);
        input = "null";
        input.push_back(';');
        std::unique_ptr<Expression> a{ ParseProgram(parser, input) };
        ASSERT_EQUAL(a->type(), ASTNodeType::kBlockStatement);
        Expression *ast = GET_STATEMENT(a, 0);
        ASSERT_EQUAL(ast->type(), ASTNodeType::kNullLiteral);
        LOG_TEST_CASE_PASSED(Syntax, CheckNullLiteral, input, count);        
    }

    TEST_TEARDOWN(Syntax, CheckNullLiteral);
}
TEST(Syntax, CheckUndefinedLiteral) {
    DECLARE_TEST(Syntax, CheckUndefinedLiteral);
    TEST_SETUP(Syntax, CheckUndefinedLiteral);

    {
        std::string input;
        int count = 1;

        LOG_TEST_CASE(Syntax, CheckUndefinedLiteral, "undefined", count);
        input = "undefined";
        input.push_back(';');
        std::unique_ptr<Expression> a{ ParseProgram(parser, input) };
        ASSERT_EQUAL(a->type(), ASTNodeType::kBlockStatement);
        Expression *ast = GET_STATEMENT(a, 0);
        ASSERT_EQUAL(ast->type(), ASTNodeType::kIdentifier);
        LOG_TEST_CASE_PASSED(Syntax, CheckUndefinedLiteral, input, count);
    }

    TEST_TEARDOWN(Syntax, CheckUndefinedLiteral);
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
    TEST_BOOL_INTERNAL(Syntax, CheckBooleanLiteral, c, count)

TEST(Syntax, CheckBooleanLiteral) {
    DECLARE_TEST(Syntax, CheckStringLiteral);
    TEST_SETUP(Syntax, CheckStringLiteral);

    {
        std::string input;
        int count = 1;

        TEST_BOOL(false);
        TEST_BOOL(true);
    }

    TEST_TEARDOWN(Syntax, CheckStringLiteral);    
}

TEST(Syntax, CheckThisHolder) {
    DECLARE_TEST(Syntax, CheckThisHolder);
    TEST_SETUP(Syntax, CheckThisHolder);

    {
        std::string input;
        int count = 1;

        LOG_TEST_CASE(Syntax, CheckThisHolder, "this", count);
        input = "this";
        input.push_back(';');
        std::unique_ptr<Expression> a{ ParseProgram(parser, input) };
        ASSERT_EQUAL(a->type(), ASTNodeType::kBlockStatement);
        Expression *ast = GET_STATEMENT(a, 0);
        ASSERT_EQUAL(ast->type(), ASTNodeType::kThisHolder);
        LOG_TEST_CASE_PASSED(Syntax, CheckThisHolder, input, count);
    }

    TEST_TEARDOWN(Syntax, CheckUndefinedLiteral);
}

#define TEST_IDENTIFIER(id) \
    do {   \
        LOG_TEST_CASE(Syntax, CheckIdentifier, id, count);  \
        input = id; \
        input.push_back(';');   \
        std::unique_ptr<Expression> a{ ParseProgram(parser, input) };   \
        ASSERT_EQUAL(a->type(), ASTNodeType::kBlockStatement);  \
        Expression *ast = GET_STATEMENT(a, 0);  \
        ASSERT_EQUAL(ast->type(), ASTNodeType::kIdentifier);    \
        ASSERT_EQUAL(ast->AsIdentifier()->GetName(), id);   \
        LOG_TEST_CASE_PASSED(Syntax, CheckIdentifier, id, count);\
    } while (0)

TEST(Syntax, CheckIdentifier) {
    DECLARE_TEST(Syntax, CheckIdentifier);
    TEST_SETUP(Syntax, CheckIdentifier);

    {
        std::string input;
        int count = 1;

        TEST_IDENTIFIER("prince");
        TEST_IDENTIFIER("_dhaliwal");
        TEST_IDENTIFIER("_0");
        TEST_IDENTIFIER("_");
        TEST_IDENTIFIER("_prince_1924023_dhaliwal");
    }

    TEST_TEARDOWN(Syntax, CheckUndefinedLiteral);
}

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
