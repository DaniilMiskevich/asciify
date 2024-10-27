#ifndef ASCII_ART_HPP
#define ASCII_ART_HPP

#include <iostream>
#include <ostream>

#include "conviniences.hpp"
#include "dims.hpp"
#include "image/image.hpp"

class AsciiArt {
    static char ascii_from_color(Image::Pixel const color) {
        static char const gradient[] = " .:-;=+*#B%@";
        static size_t const gradient_len =
            lenof(gradient) - 1 /* to account \0 */;

        let lum =
            (0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b) / 255.0;
        let ascii_idx =
            lum == 1.0 ? gradient_len - 1 : size_t(lum * gradient_len);
        return gradient[ascii_idx];
    }

   public:
    struct El {
        El() : El(Image::Pixel(0)) {}
        El(Image::Pixel const color) : c(ascii_from_color(color)) {}

        char c;

        friend std::ostream &operator<<(std::ostream &stream, El const &el) {
            return stream

                // << "\033[38;2;" << int(el.color.r) << ";" << int(el.color.g)
                // << ";" << int(el.color.b) << "m"

                << el.c;
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

    El get_el(Pos const pos) const { return data[pos.x + size.w * pos.y]; }
    void set_el(Pos const pos, El const value) {
        data[pos.x + size.w * pos.y] = value;
    }

    friend std::ostream &
    operator<<(std::ostream &stream, AsciiArt const &ascii_art) {
        for (unsigned short j = 0; j < ascii_art.size.h; j++) {
            for (unsigned short i = 0; i < ascii_art.size.w; i++)
                stream << ascii_art.get_el(Pos(i, j));
            stream << "\n";
        }

        return stream;
    }

   private:
    Size const size, char_size;
    El *const data;
};

#endif
