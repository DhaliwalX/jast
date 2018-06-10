#ifndef ERRORS_H_
#define ERRORS_H_

#include <string>

namespace jast {

// we are not using c++ exceptions because of limitations they have
class Error {
public:
  explicit Error(std::string &&message)
    : message_{ std::move(message) }
  { }
  virtual std::string Message() const { return message_; };
  virtual bool IsWarning() const { return false; };

  static Error New(std::string &&message) {
    return Error(std::forward<std::string>(message));
  }

private:
  std::string message_;
};
}

#endif
