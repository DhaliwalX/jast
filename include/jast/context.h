#ifndef CONTEXT_H_
#define CONTEXT_H_

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
private:
    ParserContextImpl *impl_;
};

}

#endif
