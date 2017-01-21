#ifndef SCANNER_H_
#define SCANNER_H_

#include <string>
#include <iosfwd>
#include <vector>

namespace jast {

// simple scanner class
template <class StreamType, class BufferType>
class BasicScanner {
public:
    BasicScanner(StreamType &is) : is_(is), buffer_(0) { }
    virtual ~BasicScanner() = default;

    // read a character from stream
    virtual int read(std::string &str, int num)  {
        str.resize(num, '\0');

        if (is_.eof())
            return -1;
        // FIXME: make it working
        // auto size = is_.readsome(str.data(), num);
        return -1;
    }

    virtual void putback(char ch) {
        buffer_.push_back(ch);
    }

    virtual char readchar() {
        if (!buffer_.empty()) {
            char ch = buffer_.back();
            buffer_.pop_back();
            return ch;
        }
        if (is_.fail() || is_.eof()) {
            return EOF;
        }
        return is_.get();
    }

private:
    StreamType &is_;
    BufferType buffer_;
};

using Scanner = BasicScanner<std::istream, std::vector<char>>;

} // jast

#endif
