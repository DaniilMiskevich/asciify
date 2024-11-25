#ifndef ASCII_ART_HPP
#define ASCII_ART_HPP

#include <stdexcept>

#include "ascii/ascii_el.hpp"
#include "dims.hpp"
#include "image/image.hpp"

class AsciiArt {
   public:
    class Iterator {
       public:
        Iterator(Pos const pos, AsciiArt const &art) : pos(pos), art(art) {}

        Pos const &get_pos() const { return pos; }

        Iterator &operator++() {
            let size = art.get_size();

            pos.x++;
            if (pos.x < size.w) return *this;

            pos.x = 0;
            pos.y++;
            if (pos.y < size.h) return *this;

            return *this;
        }

        Iterator operator++(int) {
            let it = *this;
            ++(*this);
            return it;
        }

        AsciiEl &operator*() const {
            return art.els[pos.x + pos.y * art.size.w];
        }

        bool operator==(Iterator const &other) const {
            return pos == other.pos;
        }
        bool operator!=(Iterator const &other) const {
            return pos != other.pos;
        }

       private:
        Pos pos;
        AsciiArt const &art;
    };

    AsciiArt(Image const &image, Size const char_size)
    : image(image),
      size(image.get_size() / char_size),
      els(new AsciiEl[size.get_area()]()) {}

    AsciiArt(AsciiArt const &other)
    : image(other.image),
      size(other.size),
      els(new AsciiEl[size.w * size.h]()) {
        std::copy(other.els, other.els + size.w * size.h, els);
    }

    ~AsciiArt() { delete[] els; }

    Image const &get_image() const { return image; }
    Size get_size() const { return size; };

    Iterator begin() { return Iterator(Pos(0, 0), *this); }
    Iterator end() { return Iterator(Pos(0, size.h), *this); }

    AsciiEl operator[](Pos const pos) const {
        if (pos.x >= size.w || pos.y >= size.h) return AsciiEl();
        return els[pos.x + pos.y * size.w];
    }
    AsciiEl &operator[](Pos const pos) {
        if (pos.x >= size.w || pos.y >= size.h)
            throw std::out_of_range("`pos` is outside the image.");
        return els[pos.x + pos.y * size.w];
    }

   private:
    Image const &image;

    Size const size;
    AsciiEl *const els;
};

#endif
