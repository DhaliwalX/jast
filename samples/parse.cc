#include "parser/parser.h"
#include "parser/ast-builder.h"
#include "parser/astfactory.h"
#include "parser/lexinfo.h"
#include "parser/source-locator.h"
#include "parser/token.h"

#include "pretty-printer.h"

#include <iostream>
#include <sstream>
int main()
{
    using namespace grok::parser;
    ParserContext ctx{};

    // create a LexerInfo object, will store information about string
    LexerInfo lex;

    // create a locator object this will help in printing position of errors
    SourceLocator locator(&lex);

    // ASTFactory object factory for AST nodes
    ASTFactory *factory = ASTFactory::GetFactoryInstance();

    // create an ASTBuilder object for building ast
    ASTBuilder *builder = new ASTBuilder(&ctx, factory, &locator);

    // read parse loop
    while (true) {
        std::string str;

        std::getline(std::cin, str);
        if (!std::cin)
            break;
        std::cout << "Parsing: [" << str << "]" << std::endl;
        Parser *parser = new Parser(&ctx, builder, &lex);
        Expression *ast = nullptr;
        // // create a scanner
        // std::istringstream is{ str };
        // Scanner scanner(is);

        // // pass the scanner to Lexer
        // SetScanner(&scanner);
        // while(true) {

        //     LexerInfo::advance();
        //     auto t = LexerInfo::peek();
        //     if (t == EOS || t == ILLEGAL) {
        //         std::cout << "EOS" << std::endl;
        //         break;
        //     }

        //     std::cout << Token::str(t) << std::endl;
        // }

        try {
            ast = ParseProgram(parser, str);
        } catch (std::exception &) {
            std::cout << "\x1b[33mError\x1b[0m" << std::endl;
            LexerInfo::Restart();
            delete parser;
            continue;
        }
        std::cout << "Parsed correctly" << std::endl;

        printer::PrettyPrinter p(std::cout, 1);
        ast->Accept(&p);
        delete parser;
        delete ast;
    }

    delete builder;
    return 1;
}
