#include "test.h"

namespace test {

TestDatabase RegisterTest::testDb_;

RegisterTest::RegisterTest(Test *test) {
    testDb_.addTest(test);
}

}
