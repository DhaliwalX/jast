#ifndef SOURCE_LOCATOR_H_
#define SOURCE_LOCATOR_H_

#include "jast/tokenizer.h"

#include <cctype>

namespace jast {

// SourceLocator ::= follows the current position of seek in lexer
// helpful in locating syntax errors
class SourceLocator {
public:
    SourceLocator(Tokenizer *tokenizer)
        : parent_{ tokenizer }
    { }

    virtual Position &loc();
    Position &cache() { return cache_; }

protected:
    Tokenizer *parent() { return parent_; }

private:
    Tokenizer *parent_;
    Position cache_;
};

// DummySourceLocator ::= used for testing purposes or when you don't want
// to note down position
class DummySourceLocator : public SourceLocator {
public:
    DummySourceLocator()
        : SourceLocator(nullptr)
    { }

    Position &loc()
    {
        return loc_;
    }

private:
    Position loc_;
};

}

#endif
