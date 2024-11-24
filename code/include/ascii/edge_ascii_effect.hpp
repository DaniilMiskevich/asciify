#ifndef EDGE_ASCII_EFFECT_HPP
#define EDGE_ASCII_EFFECT_HPP

#include <cassert>
#include <cstring>

#include "ascii/ascii_effect.hpp"
#include "image/filter/dog_filter.hpp"
#include "image/filter/sobel_filter.hpp"
#include "image/webp_image.hpp"

class EdgeAsciiEffect : public AsciiEffect {
   public:
    EdgeAsciiEffect(
        float const threshold,
        char const *const palette = "-\\|/",
        float const dog_eps = 1,
        float const dog_p = 10
    )
    : threshold(threshold),
      palette(palette),
      palette_len(strlen(palette)),
      dog_eps(dog_eps),
      dog_p(dog_p) {}

    void operator()(AsciiArt &dst) const override {
        let dog_filter = DoGFilter(dst.get_image(), dog_eps, dog_p);
        let sobel_filter = SobelFilter(dog_filter);

        // TODO remove later; for debugging only
        WebpImage::encode(dog_filter, "dog_filter.webp");
        WebpImage::encode(sobel_filter, "sobel_filter.webp");

        let &image = sobel_filter;
        let art_size = dst.get_size();
        let char_size = image.get_size() / art_size;

        let edge_weights = new float[palette_len]();

        letmut pos = Pos(0, 0);
        for (pos.x = 0; pos.x < art_size.w; pos.x++) {
            for (pos.y = 0; pos.y < art_size.h; pos.y++) {
                memset(edge_weights, 0, palette_len * elsizeof(edge_weights));
                for (letmut i = size_t(0); i < palette_len; i++)
                    edge_weights[i] = 0;

                letmut dpos = Pos(0, 0);
                for (dpos.x = 0; dpos.x < char_size.w; dpos.x++)
                    for (dpos.y = 0; dpos.y < char_size.h; dpos.y++) {
                        let pix = image
                            [Pos(pos.x * char_size.w, pos.y * char_size.h) +
                             dpos];

                        let angle = atan2(pix.g, pix.r) / M_PI;

                        letmut i = ssize_t(round(angle * palette_len));
                        if (i == ssize_t(palette_len))
                            i = 0;
                        else if (i < 0)
                            i = palette_len + i;

                        edge_weights[i] += pix.get_sqr_magnitude();
                    }

                letmut total_edge_weight = float(0);
                letmut max_i = size_t(0);
                for (letmut i = size_t(0); i < palette_len; i++) {
                    total_edge_weight += edge_weights[i];
                    if (edge_weights[i] < edge_weights[max_i]) max_i = i;
                }
                total_edge_weight /= char_size.get_area();

                if (total_edge_weight < threshold) continue;

                dst[pos] = AsciiEl(palette[max_i]);
            }
        }

        delete[] edge_weights;
    }

   private:
    float const threshold;
    char const *const palette;
    size_t const palette_len;
    float const dog_eps, dog_p;
};

#endif
