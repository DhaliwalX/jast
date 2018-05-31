#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <cctype>

namespace jast {

#define COUNTER_TYPE(F) \
    F(Token) \
    F(ASTNode) \
    F(InputCharacter) \
    F(Line)

class Statistics {
    enum CountType {
#define COUNT_TYPE(t) k##t,
COUNTER_TYPE(COUNT_TYPE)
#undef COUNT_TYPE
        kSize,
    };
public:
    Statistics()
        : counters_{0}
    { }

#define COUNTER_ACCESSOR(t) std::size_t &t() { return counters_[CountType::k##t]; }
COUNTER_TYPE(COUNTER_ACCESSOR)
#undef COUNTER_ACCESSOR

    void dump();
private:
    std::size_t counters_[CountType::kSize];
};

}

#endif
