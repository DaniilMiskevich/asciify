
#ifndef ASCII_EL_HPP
#define ASCII_EL_HPP

#include <ostream>

struct AsciiEl {
    AsciiEl() : AsciiEl(" ") {}

    explicit AsciiEl(std::string const data) : data(data) {}

    explicit AsciiEl(char const c) : data(std::string(1, c)) {}

    std::string data;

    friend std::ostream &operator<<(std::ostream &stream, AsciiEl const &el) {
        return stream << el.data;
    }
};

#endif
