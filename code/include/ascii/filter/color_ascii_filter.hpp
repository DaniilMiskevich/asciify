#ifndef COLOR_ASCII_FILTER_HPP
#define COLOR_ASCII_FILTER_HPP

#include <numeric>

#include "ascii/ascii_art.hpp"
#include "image/filter/image_filter.hpp"

class ColorAsciiFilter : public ImageFilter<AsciiArt> {
   public:
    ColorAsciiFilter() {}

   private:
    void operator()(AsciiArt &dst) const override {
        let &image = dst.image();
        let char_size = dst.char_size();

        for (letmut it = dst.begin(); it != dst.end(); it++) {
            let region =
                Image<Color>::Region(image, it.pos() * char_size, char_size);
            let avg = std::accumulate(region.begin(), region.end(), Color()) /
                      char_size.area();

            (*it).set_true_color(avg);
        }
    }
};

#endif
