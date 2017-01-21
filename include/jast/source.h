#ifndef SOURCE_H_
#define SOURCE_H_

#include <string>
#include <type_traits>

namespace jast {

class SourceInternal;

/*
 * Source ::= Implements source code specific details
 */
class Source {
public:
    using string_type = std::string;
    using iterator = string_type::iterator;
    using const_iterator = string_type::const_iterator;
    using size_type = string_type::size_type;
    using value_type = string_type::value_type;

#define kDefaultFile "<unnamed>"

private:
    Source(const string_type &str, const string_type &filename);

public:
    ~Source();

    size_type length() const;

    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

    value_type &operator[](size_type index);
    const value_type &operator[](size_type index) const;

    const std::string &getFileName() const;

    static Source *FromString(const std::string &str);
    static Source *FromFile(const std::string &filename);
private:
    SourceInternal *internals_;
};

// TODO: implement a class which should rather act as stream of source 

}

#endif
