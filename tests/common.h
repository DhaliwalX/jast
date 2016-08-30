#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <cassert>
#include <map>
#include <vector>
#include <chrono>

template<typename Diff>
static inline void log_progress(Diff d)
{
    std::cout << " [ " <<
        (double)std::chrono::duration_cast
        <std::chrono::microseconds>(d).count() / (double)1000
        << "ms ]" << std::endl;
}

typedef void(*test_function_t)(void);
std::map<std::string, std::vector<test_function_t>> tests;

#define INTRO(class, name) \
    std::string(#class) + "." + #name

#define ENTER_TEST_STRING(class, test_name)  \
    std::string("Entering test: ") + #class + "." + #test_name

#define END_TEST_STRING(class, name)    \
    std::string("ran ") + #class + "." + #name + " [ OK ]"

#define PASSED_TEST_STRING(class, name) \
    std::string("passed ") + #class + "." + #name

#define LOG(prefix, msg)  \
    std::cerr << "[ " << prefix << " ] " << msg << std::endl

#define LOG_START(class, name)    \
    LOG("START", ENTER_TEST_STRING(class, name))

#define LOG_END(class, name, diff)  \
    LOG(std::string("END"), END_TEST_STRING(class, name));   \
    std::cerr << "Time taken "; log_progress(diff)

#define LOG_SUCCESS(class, name)    \
    LOG("PASSED", PASSED_TEST_STRING(class, name)) 

#define ASSERT_EQUAL(a, b)  \
    assert((a == b))

#define ASSERT_NOT_EQUAL(a, b)  \
    assert(!(a == b))

#define DECLARE_TEST(class, name)

#define TEST_SETUP(class, test_name)   \
    using namespace grok::parser;   \
    auto test_start = std::chrono::high_resolution_clock::now();   \
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
    auto diff = std::chrono::high_resolution_clock::now() - test_start; \
    LOG_END(class, test_name, diff)

#define TEST(class, name)   \
    void name()


#endif
    