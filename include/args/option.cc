#include "args/option.h"

#include <cassert>

namespace jast {

std::map<std::string, BasicOption*> OptionRegistry::options;

bool OptionRegistry::Has(const std::string &name) {
    return (options.find(name) != options.end());
}

bool OptionRegistry::Get(const std::string &name) {
    return options.find(name)->second;
}

void OptionRegistry::Register(BasicOpton *option) {
    assert(option->name().length());

    options[option->name()] = option;
}

void Option::RegisterThis(BasicOption *option) {
    OptionRegistry::Register(option);
}

}
