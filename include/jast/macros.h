#ifndef MACROS_H_
#define MACROS_H_

#ifndef DISABLE_COPY
#define DISABLE_COPY(className) \
  className(const className &c) = delete; \
  className &operator=(const className &c) = delete
#endif

#endif
