#ifndef SOURCE_LOCATOR_H_
#define SOURCE_LOCATOR_H_

#include <cctype>

namespace grok {
namespace parser {
class LexerInfo;

using size_t = std::size_t;
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

    LexerInfo *parent() { return parent_; }
    SourceLocation &cache() { return cache_; }

    SourceLocation &loc();
private:
    LexerInfo *parent_;
    SourceLocation cache_;
};

}
}

#endif
