#ifndef SOURCE_LOCATOR_H_
#define SOURCE_LOCATOR_H_

#include <cctype>
#include "parser/lexinfo.h"

namespace grok {
namespace parser {

class SourceLocation final {
public:
    explicit SourceLocation(size_t pos)
        : pos_{ pos }
    { }

    size_t pos() const { return pos_; }
private:
    size_t pos_;
};

// SourceLocator ::= follows the current position of seek in lexer
// helpful in locating syntax errors
class SourceLocator {
public:
    SourceLocator(LexerInfo *lexer)
        : parent_{ lexer }, cache_{ 0 }
    { }

    virtual SourceLocation &loc();
    SourceLocation &cache() { return cache_; }

protected:
    LexerInfo *parent() { return parent_; }

private:
    LexerInfo *parent_;
    SourceLocation cache_;
};

// DummySourceLocator ::= used for testing purposes or when you don't want
// to note down position
class DummySourceLocator : public SourceLocator {
public:
    DummySourceLocator()
        : SourceLocator(nullptr), loc_{ 0 }
    { }

    SourceLocation &loc()
    {
        return loc_;
    }

private:
    SourceLocation loc_;
};

}
}

#endif
