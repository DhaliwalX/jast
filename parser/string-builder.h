#ifndef STRING_BUILDER_H_
#define STRING_BUILDER_H_

#include <string>
#include <sstream>

namespace grok {

// String ::= because std::string doesn't support utf8 so well
// but we'll use for now just std::string
using String = std::string;

class StringFactory {
public:
    static String NewString(const char *str);

    // create a new string from a given type using << operator
    template <typename T>
    static String NewStringFrom(const T &t)
    {
        std::ostringstream os;
        os << t;
        return os.str();
    }

    static String NewStringFromFile(String name);

    static String NewStringFromStd(const std::string &str);

    // and other methods
};

}


#endif
