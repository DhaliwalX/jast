### ASTBuilder
helper class for building the AST in a safe and better way. This class can do constant folding while generating AST. Thus producing shorter AST.

To create an ASTBuilder instance, you need pointer to `ParserContext`, an `ASTFactory`
instance and a `SourceLocator` object. `Parser` will use `ASTBuilder` instance to
generate AST.

```C++
class ASTBuilder {
public:
    ASTBuilder(ParserContext *ctx, ASTFactory *factory, SourceLocator *locator)
        : factory_{ factory }, locator_{ locator }, ctx_{ ctx }
    { }
    ...
    ...
};

```
Example of creating `ASTBuilder` instance

```C++

using namespace grok::parser;

// first get the instance of ASTFactory
auto factory = ASTFactory::GetInstance();

// create a LexerInfo object
LexerInfo lex;

// create a SourceLocator object
SourceLocator locator(&lex);

// now we are ready to create builder
auto builder = new ASTBuilder(&ctx, factory, &locator);

```
