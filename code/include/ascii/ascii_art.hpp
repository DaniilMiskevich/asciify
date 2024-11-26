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
        Iterator(Pos const pos, AsciiArt const &art) : _pos(pos), art(art) {}

        Pos const &pos() const { return _pos; }

        Iterator &operator++() {
            let size = art.size();

            _pos.x++;
            if (_pos.x < size.w) return *this;

            _pos.x = 0;
            _pos.y++;
            if (_pos.y < size.h) return *this;

            return *this;
        }

        Iterator operator++(int) {
            let it = *this;
            ++(*this);
            return it;
        }

        AsciiEl &operator*() const {
            return art.els[_pos.x + _pos.y * art._size.w];
        }

        bool operator==(Iterator const &other) const {
            return _pos == other._pos;
        }
        bool operator!=(Iterator const &other) const {
            return _pos != other._pos;
        }

       private:
        Pos _pos;
        AsciiArt const &art;
    };

    AsciiArt(Image const &image, Size const char_size)
    : _image(image),
      _size(image.size() / char_size),
      els(new AsciiEl[_size.area()]()) {}

    AsciiArt(AsciiArt const &other)
    : _image(other._image),
      _size(other._size),
      els(new AsciiEl[_size.area()]()) {
        std::copy(other.els, other.els + _size.area(), els);
    }

    ~AsciiArt() { delete[] els; }

    Image const &image() const { return _image; }
    Size size() const { return _size; };

    Iterator begin() { return Iterator(Pos(0, 0), *this); }
    Iterator end() { return Iterator(Pos(0, _size.h), *this); }

    AsciiEl const &operator[](Pos const pos) const {
        if (pos.x >= _size.w || pos.y >= _size.h)
            throw std::out_of_range("`pos` is outside the image.");
        return els[pos.x + pos.y * _size.w];
    }
    AsciiEl &operator[](Pos const pos) {
        if (pos.x >= _size.w || pos.y >= _size.h)
            throw std::out_of_range("`pos` is outside the image.");
        return els[pos.x + pos.y * _size.w];
    }

   private:
    Image const &_image;

    Size const _size;
    AsciiEl *const els;
};

#endif
