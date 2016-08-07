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

#define LOG_TEST_CASE(class, name, case, count)   \
    do { std::ostringstream os;  \
    std::string intro = "case #" + std::to_string(count);   \
    os << INTRO(class, name) << ": '" << case << "'";   \
    LOG(intro, os.str()); } while (0) 

#define LOG_TEST_CASE_PASSED(class, name, case, count)  \
    do { std::ostringstream os;  \
    std::string intro = "case #" + std::to_string(count++);   \
    os << INTRO(class, name) << ": '" << case << "' passed";   \
    LOG(intro, os.str());   } while(0)

#define TEST_INTEGRAL_INTERNAL(class, name, nr, count)   \
do {   \
    LOG_TEST_CASE(class, name, nr, count);  \
    input = #nr;    \
    input.push_back(';');   \
    Expression *ast = ParseProgram(parser, input);  \
    ASSERT_EQUAL(ast->type(), ASTNodeType::kBlockStatement);    \
    ast = GET_STATEMENT(ast, 0);    \
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

#define TEST_STRING_INTERNAL(class, name, case, count)  \
do {   \
    LOG_TEST_CASE(class, name, case, count);  \
    input = case;    \
    input.push_back(';');   \
    Expression *ast = ParseProgram(parser, input);  \
    ASSERT_EQUAL(ast->type(), ASTNodeType::kBlockStatement);    \
    ast = GET_STATEMENT(ast, 0);    \
    ASSERT_EQUAL(ast->type(), ASTNodeType::kStringLiteral);   \
    ASSERT_EQUAL(ast->AsStringLiteral()->string(), case);    \
    LOG_TEST_CASE_PASSED(class, name, case, count);   \
} while (0)

#define TEST_STRING(case)   \
    TEST_STRING_INTERNAL(Syntax, CheckStringLiteral, case, count)

TEST(Syntax, CheckStringLiteral) {
    DECLARE_TEST(Syntax, CheckIntegralLiteral);
    TEST_SETUP(Syntax, CheckIntegralLiteral);

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
#undef TEST_STRING_INTERNAL

}

int main()
{
    CheckIntegralLiteral();
    CheckStringLiteral();
}
