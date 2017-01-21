#ifndef COMMON_H_
#define COMMON_H_

#include "jast/token.h"

#include <stdexcept>
#include <memory>

namespace jast {

class JSError : public std::runtime_error {
public:
    JSError(std::string prefix, std::string msg)
        : std::runtime_error(std::move(msg)), prefix_{ std::move(prefix) }
    { }

    const char *what() const noexcept override
    {
        static std::string result;
        result = (prefix_ + ": " + std::runtime_error::what());
        return result.c_str();
    }
private:
    std::string prefix_;
};

class SyntaxError : public JSError {
public:
    SyntaxError(Token token, std::string str = "")
        : JSError("SyntaxError", std::move(str)), token_(token)
    { }

    const char *what() const noexcept override
    {
        return JSError::what();
    }

    Token &token() {
        return token_;
    }

private:
    Token token_;
};

class TypeError : public JSError {
public:
    TypeError(std::string msg = "")
        : JSError("TypeError", std::move(msg))
    { }

    const char *what() const noexcept override
    {
        return JSError::what();
    }
};

class ReferenceError : public JSError {
public:
    ReferenceError(std::string msg = "")
        : JSError("ReferenceError", std::move(msg))
    { }
    
    const char *what() const noexcept override
    {
        return JSError::what();
    }
};

class RangeError : public JSError {
public:
    RangeError(std::string msg = "")
        : JSError("RangeError", std::move(msg))
    { }
};

}

#endif
