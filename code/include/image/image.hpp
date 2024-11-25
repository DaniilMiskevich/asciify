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
        Iterator(Pos const pos, Image const &image) : pos(pos), image(image) {}

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

        Color operator*() const { return image[pos]; }

        bool operator==(Iterator const &other) const {
            return pos == other.pos;
        }
        bool operator!=(Iterator const &other) const {
            return pos != other.pos;
        }

       private:
        Pos pos;
        Image const &image;
    };

    virtual ~Image(){};

    virtual Size get_size(void) const = 0;
    virtual Color operator[](Pos const pos) const = 0;

    Iterator begin() { return Iterator(Pos(0, 0), *this); }
    Iterator end() { return Iterator(Pos(0, get_size().h), *this); }

    Color get_avg_in_region(Pos const pos, Size const size) const {
        letmut avg = Color();
        letmut dpos = Pos(0, 0);
        // TODO iterators + algorithms
        for (dpos.x = 0; dpos.x < size.w; dpos.x++)
            for (dpos.y = 0; dpos.y < size.h; dpos.y++) {
                let pix = (*this)[Pos(pos.x * size.w, pos.y * size.h) + dpos];
                avg += pix;
            }

        let char_area = size.get_area();
        avg /= char_area;

        return avg;
    }

   public:
    class LoadingException;
};

class Image::LoadingException : public std::exception {
   public:
    virtual char const *what() const throw() override = 0;
};

#endif
