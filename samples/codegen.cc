#include "jast/parser-builder.h"
#include "codegen/codegen.h"

#include <iostream>
#include <sstream>
int main()
{
    using namespace jast;
    CodeGenerator::Initialize();

    ParserBuilder builder(std::cin, "STDIN");
    Parser *parser = builder.parser();
    Handle<Expression> ast = nullptr;

    try {
        ast = ParseProgram(parser);
    } catch (std::exception &) {
        std::cout << "\x1b[33mError\x1b[0m" << std::endl;
        return -1;
    }
    std::cout << "Parsed correctly" << std::endl;

    CodeGenerator codegen;
    ast->Accept(&codegen);

    return 0;
}
