#ifndef PARSER_BUILDER_H_
#define PARSER_BUILDER_H_

#include <memory>
#include "jast/parser.h"
#include "jast/ast-builder.h"
#include "jast/astfactory.h"
#include "jast/source-locator.h"
#include "jast/token.h"
#include "jast/scope.h"

namespace jast {

class ParserBuilder {
public:
    ParserBuilder(std::istream &is, const std::string &filename = "STDIN")
    :
        context_{ std::make_unique<ParserContext>() },
        stream_{ std::make_unique<StandardCharacterStream>(is) },
        lex_{ std::make_unique<Tokenizer>(stream_.get(), context_.get()) },
        locator_{ std::make_unique<SourceLocator>(lex_.get()) },
        factory_{ ASTFactory::GetFactoryInstance() },
        manager_{ std::make_unique<ScopeManager>(context_.get()) },
        builder_{ std::make_unique<ASTBuilder>(context_.get(), factory_, locator_.get(), manager_.get()) },
        parser_{ std::make_unique<Parser>(context_.get(), builder_.get(), lex_.get(), manager_.get()) },
        filename_{ filename }
    { }

    Parser *Build() {
        return parser_.get();
    }

    ParserContext *context() { return context_.get(); }

private:
    std::unique_ptr<ParserContext> context_;
    std::unique_ptr<CharacterStream> stream_;
    std::unique_ptr<Tokenizer> lex_;
    std::unique_ptr<SourceLocator> locator_;
    ASTFactory* factory_;
    std::unique_ptr<ScopeManager> manager_;
    std::unique_ptr<ASTBuilder> builder_;
    std::unique_ptr<Parser> parser_;
    std::string filename_;
};

}

#endif
