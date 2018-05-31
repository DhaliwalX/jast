#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "jast/statistics.h"

namespace jast {

class Scope;
class ParserContextImpl;

// class to store complete context of parser independant of
// actual interpreter.
class ParserContext {
public:
    ParserContext();
    ~ParserContext();

    Scope *GetGlobalScope();

    Statistics &Counters();
private:
    ParserContextImpl *impl_;
    Statistics statistics_;
};

}

#endif
