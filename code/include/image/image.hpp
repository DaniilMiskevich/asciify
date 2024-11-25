#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <exception>

#include "color.hpp"
#include "conviniences.hpp"
#include "dims.hpp"

class Image {
   public:
    class LoadingException;

    virtual ~Image(){};
    // TODO? move + copy

    virtual Size get_size(void) const = 0;
    virtual Color operator[](Pos const pos) const = 0;

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
};

class Image::LoadingException : public std::exception {
   public:
    virtual char const *what() const throw() override = 0;
};

#endif
