#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <exception>

#include "color.hpp"
#include "conviniences.hpp"
#include "dims.hpp"

class Image {
   public:
    class Iterator {
       public:
        Iterator(Pos const pos, Image const &image)
        : pos(pos), size(image.size()), image(image) {}
        Iterator(Pos const pos, Image const &image, Size const region_size)
        : pos(pos), size(region_size), image(image) {}

        Pos const &get_pos() const { return pos; }

        Iterator &operator++() {
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

        Color const &operator*() const { return image[pos]; }

        bool operator==(Iterator const &other) const {
            return pos == other.pos;
        }
        bool operator!=(Iterator const &other) const {
            return pos != other.pos;
        }

       private:
        Pos pos;
        Size const size;
        Image const &image;
    };

    class Region;

    virtual ~Image(){};

    virtual Size size(void) const = 0;
    virtual Color const &operator[](Pos const pos) const = 0;

    Iterator begin() const { return Iterator(Pos(0, 0), *this); }
    Iterator end() const { return Iterator(Pos(0, size().h), *this); }

   public:
    class LoadingException;
};

class Image::Region : public Image {
   public:
    Region(Image const &base, Pos const offset, Size const size)
    : offset(offset), _size(size), base(base) {}

    Size size() const override { return _size; }
    Color const &operator[](Pos const pos) const override {
        return base[offset + pos];
    }

   private:
    Pos const offset;
    Size const _size;
    Image const &base;
};

class Image::LoadingException : public std::exception {
   public:
    virtual char const *what() const throw() override = 0;
};

#endif
