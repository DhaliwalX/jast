#include "jast/parser-builder.h"
#include "pretty-printer.h"

#include <iostream>
#include <sstream>
int main()
{
    using namespace jast;

    ParserBuilder builder(std::cin, "STDIN");
    Parser *parser = builder.parser();
    Expression *ast = nullptr;

    try {
        ast = ParseProgram(parser);
    } catch (std::exception &) {
        std::cout << "\x1b[33mError\x1b[0m" << std::endl;
        return -1;
    }
    std::cout << "Parsed correctly" << std::endl;

    printer::PrettyPrinter p(std::cout, 1);
    // ast->Accept(&p);

    delete ast;
    return 0;
}
