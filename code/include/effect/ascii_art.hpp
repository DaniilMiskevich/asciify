#ifndef ASCII_ART_HPP
#define ASCII_ART_HPP

#include <iostream>
#include <ostream>

#include "dims.hpp"

class AsciiArt {
   public:
    struct El {
        El() : El(" ") {}

        El(std::string const data) : data(data) {}

        El(char const c) : data(std::string(1, c)) {}

        std::string data;

        friend std::ostream &operator<<(std::ostream &stream, El const &el) {
            return stream << el.data;
        }
    };

    AsciiArt() = delete;

    AsciiArt(Size const image_size, Size const char_size)
    : size(image_size / char_size),
      char_size(char_size),
      data(new El[size.get_area()]) {}

    ~AsciiArt() { delete[] data; }

    Size get_size() const { return size; }
    Size get_char_size() const { return char_size; }

    El const &operator[](Pos const pos) const {
        return data[pos.x + size.w * pos.y];
    }
    El &operator[](Pos const pos) { return data[pos.x + size.w * pos.y]; }

    friend std::ostream &
    operator<<(std::ostream &stream, AsciiArt const &ascii_art) {
        for (unsigned short j = 0; j < ascii_art.size.h; j++) {
            for (unsigned short i = 0; i < ascii_art.size.w; i++)
                stream << ascii_art[Pos(i, j)];
            stream << "\n";
        }

        return stream;
    }

   private:
    Size const size, char_size;
    El *const data;
};

#endif
