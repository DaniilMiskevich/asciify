#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "color.hpp"
#include "conviniences.hpp"
#include "dims.hpp"

class Image {
   public:
    virtual ~Image(){};

    virtual Size get_size(void) const = 0;
    virtual Color operator[](Pos const pos) const = 0;

    Color get_avg_in_region(Pos const pos, Size const size) const {
        letmut avg_r = 0.0, avg_g = 0.0, avg_b = 0.0;
        for (letmut dx = 0; dx < size.w; dx++)
            for (letmut dy = 0; dy < size.h; dy++) {
                let pix =
                    (*this)[Pos(pos.x * size.w + dx, pos.y * size.h + dy)];
                avg_r += pix.r, avg_g += pix.g, avg_b += pix.b;
            }

        let char_area = size.get_area();
        avg_r /= char_area, avg_g /= char_area, avg_b /= char_area;

        return Color(avg_r, avg_g, avg_b);
    }

    template <uint16_t W, uint16_t H>
    static double
    filter(Color const (&src)[W][H], double const (&kernel)[W][H]) {
        letmut s = 0.0;
        for (letmut i = uint16_t(0); i < W; i++)
            for (letmut j = uint16_t(0); j < H; j++)
                s += src[i][j].get_luminance() * kernel[i][j];

        return s;
    }
};

#endif
