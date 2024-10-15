#ifndef ASCII_ART_HPP
#define ASCII_ART_HPP

#include <ostream>

#include "dims.hpp"

class AsciiArt {
   public:
    struct El {
        char c;
        bool is_transparent;

        friend std::ostream &operator<<(std::ostream &stream, El const &el) {
            return stream << el.c;
        }
    };

    AsciiArt() = delete;
    AsciiArt(Size const size) : size(size), data(new El[size.get_area()]) {}
    ~AsciiArt() { delete[] data; }

    Size get_size() const { return size; }
    El get_el(Pos const pos) const { return data[pos.x + size.w * pos.y]; }
    void set_el(Pos const pos, El const value) {
        data[pos.x + size.w * pos.y] = value;
    }

    friend std::ostream &
    operator<<(std::ostream &stream, AsciiArt const &ascii_art) {
        for (unsigned short i = 0; i < ascii_art.size.w; i++) {
            for (unsigned short j = 0; j < ascii_art.size.w; j++)
                stream << ascii_art.get_el(Pos(i, j));
            stream << "\n";
        }

        return stream;
    }

   private:
    Size const size;
    El *const data;
};

#endif
