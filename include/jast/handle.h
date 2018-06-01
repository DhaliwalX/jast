#ifndef HANDLE_H_
#define HANDLE_H_

#include <memory>

namespace jast {

template <typename T>
using Handle = std::shared_ptr<T>;

template <typename T, typename... Args>
Handle<T> MakeHandle(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}

#endif
