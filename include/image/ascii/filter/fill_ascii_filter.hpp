#ifndef FILL_ASCII_FILTER_HPP
#define FILL_ASCII_FILTER_HPP

#include <cstring>
#include <numeric>

#include "conviniences.hpp"
#include "image/ascii/filter/ascii_filter.hpp"

class FillAsciiFilter : public AsciiFilter {
   public:
    explicit FillAsciiFilter(char const *const palette = " .:-;=+*#B%@")
    : palette(palette), palette_len(strlen(palette)) {}

   private:
    char const *const palette;
    size_t const palette_len;

    void operator()(Ascii &dst) const override {
        let &bitmap = dst.bitmap();
        let char_size = dst.char_size();

        for (letmut it = dst.begin(); it != dst.end(); it++) {
            let region =
                Bitmap::Region(bitmap, it.pos() * char_size, char_size);
            let avg = std::accumulate(region.begin(), region.end(), Color()) /
                      char_size.area();
            let avg_lum = avg.luminance();

            letmut i = size_t(avg_lum * palette_len);
            if (i >= palette_len) i = palette_len - 1;

            *it = AsciiEl(palette[i]);
        }
    }
};

#endif
