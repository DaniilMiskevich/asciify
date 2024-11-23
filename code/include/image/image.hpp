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

    virtual Size get_size(void) const = 0;
    virtual Color operator[](Pos const pos) const = 0;

    Color get_avg_in_region(Pos const pos, Size const size) const {
        letmut avg = Color();
        for (letmut dx = 0; dx < size.w; dx++)
            for (letmut dy = 0; dy < size.h; dy++) {
                let pix =
                    (*this)[Pos(pos.x * size.w + dx, pos.y * size.h + dy)];
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
