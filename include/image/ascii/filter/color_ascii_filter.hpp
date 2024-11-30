#ifndef COLOR_ASCII_FILTER_HPP
#define COLOR_ASCII_FILTER_HPP

#include <numeric>

#include "image/ascii/filter/ascii_filter.hpp"

class ColorAsciiFilter : public AsciiFilter {
   private:
    void operator()(Ascii &dst) const override {
        let &bitmap = dst.bitmap();
        let char_size = dst.char_size();

        for (letmut it = dst.begin(); it != dst.end(); it++) {
            let region =
                Image<Color>::Region(bitmap, it.pos() * char_size, char_size);
            let avg = std::accumulate(region.begin(), region.end(), Color()) /
                      char_size.area();

            (*it).set_true_color(avg);
        }
    }
};

#endif
