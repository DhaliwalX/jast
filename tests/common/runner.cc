#include "ast-builder/ast-builder-test.h"
#include "parser/context.h"
#include <string>
#include <iostream>
#include <map>

int main() {
    using namespace test;
    std::map<std::string, Test*> tests;

    tests.insert({ "AstBuilder", new AstBuilderTest()});

    grok::parser::ParserContext context;
    for (auto &t : tests) {
        t.second->init(&context);
        bool result = t.second->run();

        if (!result) {
            return -1;
        }

        t.second->destroy();
        delete t.second;
    }

    return 0;
}
