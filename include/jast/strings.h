#ifndef STRINGS_H_
#define STRINGS_H_

#include <string>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace jast {

class Strings {
public:
  template <typename... Args>
  static std::string Format(fmt::string_view &&view, Args&&... args) {
    return fmt::format(view, std::forward<Args>(args)...);
  }

  static std::string Raw(std::string &&view) {
    return view;
  }
};

}

#endif
