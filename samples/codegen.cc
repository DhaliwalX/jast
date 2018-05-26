#include "jast/parser-builder.h"
#include "codegen/codegen.h"

#include "jit/simple.h"

#include <iostream>
#include <sstream>


extern "C" void print(double val) {
    std::cout << val << std::endl;
}


int main()
{
    using namespace jast;
    CodeGenerator::Initialize();

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

    CodeGenerator codegen;
    ast->Accept(&codegen);

    auto m = codegen.ReleaseModule();
    m->dump();

    using namespace llvm::orc;
    std::unique_ptr<JIT> jit = std::make_unique<JIT>();
    jit->addModule(std::move(m));

    auto symbol = jit->findSymbol("__init__");

    void (*func)() = (void(*)())(intptr_t)symbol.getAddress();
    if (func == nullptr) {
        std::cout << "Func in nullptr" << std::endl;
    } else {
        func();
    }
    delete ast;
    return 0;
}
