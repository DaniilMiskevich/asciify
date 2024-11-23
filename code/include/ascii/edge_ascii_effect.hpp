#ifndef EDGE_ASCII_EFFECT_HPP
#define EDGE_ASCII_EFFECT_HPP

#include <cassert>
#include <cstring>

#include "ascii/ascii_effect.hpp"
#include "image/filter/sobel_filter.hpp"

class EdgeAsciiEffect : public AsciiEffect {
   public:
    EdgeAsciiEffect(float const threshold, char const *const palette = "-/|\\")
    : threshold(threshold), palette(palette) {
        assert(strlen(palette) == 4);
    }

    void operator()(AsciiArt &dst) const override {
        let &image = SobelFilteredImage(dst.get_image());
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
