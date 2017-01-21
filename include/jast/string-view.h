#ifndef STRING_VIEW_H_
#define STRING_VIEW_H_

#include <string>

template <class String>
class BasicStringView {
public:
    using iterator = typename String::iterator;
    using const_iterator = typename String::const_iterator;
    using reverse_iterator = typename String::reverse_iterator;
    using value_type = typename String::value_type;
    using reference = typename String::reference;
    using const_reference = typename String::const_reference;
    using size_type = typename String::size_type;
    using difference_type = typename String::difference_type;

    BasicStringView(iterator start, iterator end)
        : start_{start}, end_{end}
    { }

    iterator &begin() {
        return start_;
    }

    iterator &end() {
        return end_;
    }

    const_iterator cbegin() const {
        return start_;
    }

    const_iterator cend() const {
        return end_;
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end_);
    }

    reverse_iterator rend() {
        return reverse_iterator(start_);
    }

    size_type size() {
        return (size_type)(end_ - start_);
    }

    size_type length() {
        return size();
    }

    bool empty() {
        return start_ == end_;
    }

    value_type &operator[](difference_type off) {
        return *(start_ + off);
    }

    const value_type &operator[](difference_type off) const {
        return *(start_ + off);
    }

    value_type &at(difference_type off) {
        return *(start_ + off);
    }

    const value_type &at(difference_type off) const {
        return *(start_ + off);
    }

private:
    iterator start_;
    iterator end_;
};

using StringView = BasicStringView<std::string>;

#endif
