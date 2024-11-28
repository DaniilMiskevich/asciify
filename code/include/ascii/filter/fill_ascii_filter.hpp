#ifndef FILL_ASCII_FILTER_HPP
#define FILL_ASCII_FILTER_HPP

#include <cstring>
#include <numeric>

#include "ascii/ascii_art.hpp"
#include "conviniences.hpp"
#include "image/filter/image_filter.hpp"
#include "image/image.hpp"

class FillAsciiFilter : public ImageFilter<AsciiArt> {
   public:
    FillAsciiFilter(char const *const palette = " .:-;=+*#B%@")
    : palette(palette), palette_len(strlen(palette)) {}

   private:
    char const *const palette;
    size_t const palette_len;

    void operator()(AsciiArt &dst) const override {
        let &image = dst.image();
        let char_size = dst.char_size();

        for (letmut it = dst.begin(); it != dst.end(); it++) {
            let region =
                Image<Color>::Region(image, it.pos() * char_size, char_size);
            let avg = std::accumulate(region.begin(), region.end(), Color()) /
                      char_size.area();
            let avg_lum = avg.luminance();

            letmut i = size_t(avg_lum * palette_len);
            if (i >= palette_len) i = palette_len - 1;

            (*it) = AsciiEl(palette[i]);
        }
    }
};

#endif
