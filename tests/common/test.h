#ifndef TEST_H_
#define TEST_H_

#include "parser/context.h"

namespace test {

// abstract base class for writing test classes
//   - initialize test in its constructor
//   - destroy test in its destructor
class Test {
public:
    virtual ~Test() {}

    // main run function
    virtual bool run() = 0;

    // main init function
    virtual void init(grok::parser::ParserContext *context) {
        context_ = context;
    }

    // destroy the test object
    virtual void destroy() = 0;
private:
    grok::parser::ParserContext *context_;
};

}

#endif // TEST_H_
