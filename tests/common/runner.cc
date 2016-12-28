#include "test.h"
#include "parser/context.h"
#include <iostream>

int main() {
    using namespace test;
    
    auto &db = RegisterTest::getDatabase();
    auto &tests = db.getAll();

    grok::parser::ParserContext context;
    for (auto &test : tests) {
        test->init(&context);
        test->run();
        test->destroy();
    }
    return 0;
}
