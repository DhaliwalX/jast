#ifndef TEST_H_
#define TEST_H_

#include "parser/context.h"
#include <vector>

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

class TestDatabase {
    std::vector<Test*> tests_;

public:
    void addTest(Test *test) {
        tests_.push_back(test);
    }

    std::vector<Test*>& getAll() {
        return tests_;
    }

    ~TestDatabase() {
        for (auto &test : tests_) {
            delete test;
        }
    }
};

class RegisterTest {
    static TestDatabase testDb_;
public:
    // constructor is enough for everything
    RegisterTest(Test *test);

    static TestDatabase &getDatabase() {
        return testDb_;
    }
};

}

#endif // TEST_H_
