#include "jast/parser.h"
#include "jast/ast-builder.h"
#include "jast/astfactory.h"
#include "jast/source-locator.h"
#include "jast/token.h"

#include "pretty-printer.h"

#include <iostream>
#include <sstream>
int main()
{
    using namespace jast;
    ParserContext ctx{};


    // ASTFactory object factory for AST nodes
    ASTFactory *factory = ASTFactory::GetFactoryInstance();


    Scanner scanner(std::cin);

    // create a Tokenizer object, will store information about string
    Tokenizer lex(&scanner);

    // create a locator object this will help in printing position of errors
    SourceLocator locator(&lex);

    // create an ASTBuilder object for building ast
    ASTBuilder *builder = new ASTBuilder(&ctx, factory, &locator);

    Parser *parser = new Parser(&ctx, builder, &lex);
    Expression *ast = nullptr;
    // // create a scanner
    // std::istringstream is{ str };
    // Scanner scanner(is);

    // // pass the scanner to Lexer
    // SetScanner(&scanner);
    // while(true) {

    //     Tokenizer::advance();
    //     auto t = Tokenizer::peek();
    //     if (t == EOS || t == ILLEGAL) {
    //         std::cout << "EOS" << std::endl;
    //         break;
    //     }

    //     std::cout << Token::str(t) << std::endl;
    // }

    try {
        ast = ParseProgram(parser);
    } catch (std::exception &) {
        std::cout << "\x1b[33mError\x1b[0m" << std::endl;
        delete parser;
        delete builder;
        return -1;
    }
    std::cout << "Parsed correctly" << std::endl;

    printer::PrettyPrinter p(std::cout, 1);
    ast->Accept(&p);

    // clean up
    delete parser;
    delete ast;
    delete builder;
    return 0;
}
