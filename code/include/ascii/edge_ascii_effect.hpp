#ifndef EDGE_ASCII_EFFECT_HPP
#define EDGE_ASCII_EFFECT_HPP

#include <cassert>
#include <cstring>

#include "ascii/ascii_effect.hpp"

class SobelFilter : public Image {
   public:
    SobelFilter(Image const &src) : src(src) {}

    Size get_size(void) const { return src.get_size(); }
    // TODO? maybe worth to compute once in the constructor
    Color operator[](Pos const pos) const {
        static double const gx[3][3] = {
            {+1, +0, -1},
            {+2, +0, -2},
            {+1, +0, -1},
        };
        static double const gy[3][3] = {
            {+1, +2, +1},
            {+0, +0, +0},
            {-1, -2, -1},
        };
        double const src_pxs[3][3] = {
            {
                src[pos - 1].get_magnitude(),
                src[pos + Pos(0, -1)].get_magnitude(),
                src[pos + Pos(1, -1)].get_magnitude(),
            },
            {
                src[pos + Pos(-1, 0)].get_magnitude(),
                src[pos].get_magnitude(),
                src[pos + Pos(1, 0)].get_magnitude(),
            },
            {
                src[pos + Pos(-1, 1)].get_magnitude(),
                src[pos + Pos(0, 1)].get_magnitude(),
                src[pos + 1].get_magnitude(),
            },
        };

        let x = Image::filter(src_pxs, gx), y = Image::filter(src_pxs, gy);

        return Color(x, y, 0);
    }

   private:
    Image const &src;
};

class EdgeAsciiEffect : public AsciiEffect {
   public:
    EdgeAsciiEffect(float const threshold, char const *const palette = "-/|\\")
    : threshold(threshold), palette(palette) {
        assert(strlen(palette) == 4);
    }

    void operator()(AsciiArt &dst) const override {
        let &image = SobelFilter(dst.get_image());
        let size = dst.get_size();
        let char_size = image.get_size() / size;

        letmut pos = Pos(0, 0);
        for (pos.x = 0; pos.x < size.w; pos.x++) {
            for (pos.y = 0; pos.y < size.h; pos.y++) {
                let avg_col = image.get_avg_in_region(pos, char_size);
                if (fabs(avg_col.get_magnitude()) <= threshold) continue;

                let angle = atan2(avg_col.r, avg_col.g) / M_PI;
                letmut i = int(round(angle * 4));
                if (i < 0) i += 4;
                if (i == 4) i = 0;

                dst[pos] = AsciiEl(palette[i]);
            }
        }
    }

   private:
    float const threshold;
    char const *const palette;
};

#endif
