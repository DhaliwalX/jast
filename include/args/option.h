#ifndef OPTION_H_

#include "args/option-parser.h"

#include <string>

namespace jast {

class BasicOption {
public:
    virtual ~BasicOption() = default;

    virtual std::string &name() const = 0;
    virtual std::string ToString() const = 0;
    virtual bool Parse(const std::string &option) = 0;
};

class OptionRegistry {
private:
    static std::map<std::string, BasicOption*> options;

public:
    static bool Has(const std::string &name);
    static BasicOption* Get(const std::string &name);
    static void Register(BasicOption *option);
};

template <typename DataType, typename ParserType>
class Option : public BasicOption {
public:
    Option(std::string name, std::string description,
                    DataType default_value = DataType())
    : name_{name}, description_{description}, value_{default_value}
    {
        RegisterThis(this);
    }

    Option(const Option&) = default;
    Option(Option&&) = default;
    Option &operator=(const Option &) = default;
    Option &operator=(Option&&) = default;

    std::string &name() override const { return name_; }
    DataType &value() { return value_; }

    std::string ToString() const override {
        return ParserType().ToString(value_);
    }

    bool Parse(const std::string &option) override {
        bool status = ParserType().Parse(option, &value_);
        value_ = status ? value_ : DataType();
        return status;
    }

private:
    static void RegisterThis(BasicOption *option);

    std::string name_;
    std::string description_;
    DataType value_;
};

}

#endif
