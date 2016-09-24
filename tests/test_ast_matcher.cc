#include "parser/ast-builder.h"
#include "parser/parser.h"
#include "parser/ast-builder.h"
#include "parser/astfactory.h"
#include "parser/lexinfo.h"
#include "parser/source-locator.h"
#include "parser/token.h"
#include "parser/ast-match.h"
#include "./common.h"

#include <cassert>
#include <ctime>

using namespace grok::parser;
namespace test {

ASTBuilder *MakeBuilder(ParserContext *context, SourceLocator *locator)
{
    return new ASTBuilder(context, ASTFactory::GetFactoryInstance(), locator);
}

class Logger {
public:
    enum LogLevel {
        kNormal,
        kSevere,
        kWarning,
        kDevWarning
    };

    static const LogLevel kDefaultLevel = kNormal;

    Logger(std::ostream &os) 
    : os_ {os} 
    { }

    virtual void Log(std::string message, LogLevel level = kDefaultLevel) {
        std::time_t t;
        time(&t);
        os() << ctime(&t) << ": " << message << std::endl;
    }

    std::ostream &os() { return os_; }
private:
    std::ostream &os_;
};

// class StaticTestingParser {
// public:
//     static void Init() {
//         factory_ = ASTFactory::GetInstance();
//         builder_ = new ASTBuilder(&context_, factory_, &locator_);
//         parser_ = new Parser(&context_, builder_, &lex_);
//     }

//     static Expression *ParseString(std::string &str) {
//         return ParseProgram(parser, str);
//     }

//     static void ShutDown() {
//         delete factory_;
//         delete builder_;
//         delete parser_;
//     }
//     static ParserContext context_;
// private:
//     static LexerInfo lex_;
//     static SourceLocator locator_;
//     static ASTFactory *factory_;
//     static ASTBuilder *builder_;
//     static Parser *parser_;
// };

// ParserContext StaticTestingParser::context_{};
// LexerInfo StaticTestingParser::lex_{};
// SourceLocator StaticTestingParser::locator_(&lex_);
// ASTFactory *StaticTestingParser::factory_ = nullptr;
// ASTBuilder *StaticTestingParser::builder_ = nullptr;
// Parser *StaticTestingParser::parser_ = nullptr;

}

#define N(exprType) builder->New ## exprType()
#define MATCH() assert(matcher.match(a, b))
#define NMATCH() assert(!matcher.match(a, b))

int main() {
    using namespace test;
    // StaticTestingParser::Init();

    ParserContext context_;
    DummySourceLocator locator;
    ASTBuilder *builder = MakeBuilder(&context_,
                                      &locator);
    FastASTMatcher matcher;
    Logger logger(std::cout);
    // Testing stuff goes here
    {

        // test for NullLiteral, UndefinedLiteral and ThisHolder
        Expression *a = N(NullLiteral);
        Expression *b = N(NullLiteral);
        assert(matcher.match(a, b));
        logger.Log("Passed test for NullLiteral");
        delete a;
        delete b;

        a = N(UndefinedLiteral);
        b = N(UndefinedLiteral);
        assert(matcher.match(a, b));
        logger.Log("Passed test for UndefinedLiteral");
        delete a;
        delete b;

        a = N(ThisHolder);
        b = N(ThisHolder);
        assert(matcher.match(a, b));
        logger.Log("Passed test for ThisHolder");   
        delete a;
        delete b;
    }

    {
        // test for Primitives
        Expression *a = builder->NewIntegralLiteral(10);
        Expression *b = builder->NewIntegralLiteral(20);
        assert(!matcher.match(a, b));
        delete b;
        b = builder->NewIntegralLiteral(10);
        assert(matcher.match(a, b));
        delete a;
        delete b;
        logger.Log("Passed test for IntegralLiteral");

        a = builder->NewStringLiteral("Prince Dhaliwal");
        b = builder->NewStringLiteral("Prince Dhaliwal");
        MATCH();
        delete a;
        a = builder->NewStringLiteral("Hello");
        NMATCH();
        delete a;
        delete b;
        logger.Log("Passed test for StringLiteral");

        a = builder->NewIdentifier("a");
        b = builder->NewIdentifier("b");
        NMATCH();
        delete a;
        a = builder->NewIdentifier("b");
        MATCH();
        delete a;
        delete b;
        logger.Log("Passed test for Identifier");
    }

    {
        // test for Expressions
        
    }

    delete builder;

    // StaticTestingParser::ShutDown();
}
