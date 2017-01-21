#include "jast/source.h"

#include <cassert>
#include <cstdio>

namespace jast {

class SourceInternal {
public:
    friend class Source;

    using string_type = Source::string_type;
    using size_type = Source::size_type;
    using iterator = string_type::iterator;
    using const_iterator = string_type::const_iterator;
    using value_type = string_type::value_type;

    SourceInternal(const std::string &str, const string_type &filename)
    : str_{ str }, filename_{filename}
    { }

    inline value_type &get(size_type index) {
        assert(index >= 0 && index < str_.length());
        return str_[index];
    }

    inline size_type length() {
        return str_.length();
    }

    inline iterator begin() {
        return str_.begin();
    }

    inline iterator end() {
        return str_.end();
    }

    inline const_iterator cbegin() const {
        return str_.cbegin();
    }

    inline const_iterator cend() const {
        return str_.cend();
    }

private:
    string_type str_;
    string_type filename_;
};

Source::Source(const string_type &source, const string_type &filename)
: internals_{ new SourceInternal(source, filename) }
{ }

Source::~Source() {
    delete internals_;
}

Source::size_type Source::length() const {
    return internals_->length();
}

Source::iterator Source::begin() {
    return internals_->begin();
}

Source::iterator Source::end() {
    return internals_->end();
}

Source::const_iterator Source::cbegin() const {
    return internals_->cbegin();
}

Source::const_iterator Source::cend() const {
    return internals_->cend();
}

Source::value_type &Source::operator[](size_type index) {
    return internals_->get(index);
}

const Source::value_type &Source::operator[](size_type index) const {
    return internals_->get(index);
}

const std::string &Source::getFileName() const {
    return internals_->filename_;
}

Source *Source::FromString(const std::string &str) {
    return new Source(str, string_type(kDefaultFile));
}

Source *Source::FromFile(const std::string &filename) {
    string_type v;
    if (FILE *fp = fopen(filename.c_str(), "r"))
    {
        char buf[1024];
        while (size_t len = fread(buf, 1, sizeof(buf), fp))
            v.insert(v.end(), buf, buf + len);
        fclose(fp);
    }

    return FromString(v);
}

} // jast
