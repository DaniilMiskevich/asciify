#ifndef ASCII_ART_HPP
#define ASCII_ART_HPP

#include <ostream>

#include "ascii/ascii_el.hpp"
#include "dims.hpp"
#include "image/image.hpp"

class AsciiArt {
   public:
    AsciiArt(Image const &image, Size const char_size)
    : image(image),
      size(image.get_size() / char_size),
      els(new AsciiEl[size.get_area()]()) {}

    ~AsciiArt() { delete[] els; }

    Image const &get_image() { return image; }
    Size get_size() const { return size; };

    AsciiEl operator[](Pos const pos) const {
        if (pos.x >= size.w || pos.y >= size.h) return AsciiEl();
        return els[pos.x + pos.y * size.w];
    }
    AsciiEl &operator[](Pos const pos) {
        if (pos.x >= size.w || pos.y >= size.h)
            throw std::out_of_range("`pos` is outside the image.");
        return els[pos.x + pos.y * size.w];
    }

    friend std::ostream &
    operator<<(std::ostream &stream, AsciiArt const &ascii_art);

   private:
    Image const &image;
    Size const size;

    AsciiEl *const els;
};

#endif
