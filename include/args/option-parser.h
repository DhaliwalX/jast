#ifndef OPTION_PARSER_H_
#define OPTION_PARSER_H_

#include <string>

namespace jast {

class Int32OptionParser {
public:
    std::string ToString(int32_t value) {
        return std::to_string(value);
    }

    bool Parse(const std::string &option, int32_t *value) {
        try {
            *value = std::stoi(option);
        } catch (...) {
            return false;
        }

        return true;
    }
};

}

#endif
