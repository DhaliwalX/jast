#include <jast/scanner.h>

#include <gtest/gtest.h>

#include <sstream>
#include <cassert>

using namespace jast;

#define Expect(ch)  \
    ASSERT_EQ(scanner->readchar(), ch)

namespace {

TEST(ScannerTest, normalScanning) {
    std::istringstream stream("prince");
    Scanner *scanner = new Scanner(stream);
    Expect('p');
    Expect('r');
    Expect('i');
    Expect('n');
    Expect('c');
    Expect('e');
    Expect(EOF);
    Expect(EOF);

    // testing putback functionality
    scanner->putback(EOF);
    Expect(EOF);
    scanner->putback('p');
    Expect('p');
    scanner->putback('\n');
    Expect('\n');

    scanner->putback('a');
    scanner->putback('b');
    scanner->putback('d');

    Expect('d');
    Expect('b');
    Expect('a');
    Expect(EOF);
    delete scanner;
}

}

