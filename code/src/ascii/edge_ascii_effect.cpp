#include "ascii/effect/edge_ascii_effect.hpp"

#include <numeric>

#include <alloca.h>

#include "image/filter/dog_filter.hpp"
#include "image/filter/sobel_filter.hpp"
#include "image/webp_image.hpp"

void EdgeAsciiEffect::operator()(AsciiArt &dst) const {
    let dog_filter = DoGFilter(dst.get_image(), dog_eps, dog_p);
    let sobel_filter = SobelFilter(dog_filter);

    // TODO remove later; for debugging only
    WebpImage::encode(dog_filter, "dog_filter.webp");
    WebpImage::encode(sobel_filter, "sobel_filter.webp");

    let &image = sobel_filter;
    let art_size = dst.get_size();
    let char_size = image.get_size() / art_size;

    float *const edge_weights =
        reinterpret_cast<float *>(alloca(elsizeof(edge_weights) * palette_len));

    for (letmut it = dst.begin(); it != dst.end(); it++) {
        std::fill(edge_weights, edge_weights + palette_len, 0);

        letmut dpos = Pos(0, 0);
        // TODO iterators
        for (dpos.x = 0; dpos.x < char_size.w; dpos.x++)
            for (dpos.y = 0; dpos.y < char_size.h; dpos.y++) {
                let pos = it.get_pos();
                let pix =
                    image[Pos(pos.x * char_size.w, pos.y * char_size.h) + dpos];

                // must be negated to preserve palette in normal order
                let angle = -atan2(pix.g, pix.r) / M_PI;

                letmut i = size_t(round(angle * palette_len));
                if (i == palette_len)
                    i = 0;
                else if (i > palette_len)
                    i = palette_len + i;

                edge_weights[i] += pix.get_sqr_magnitude();
            }

        let total_edge_weight =
            std::accumulate(edge_weights, edge_weights + palette_len, 0.0);
        if (total_edge_weight / char_size.get_area() < threshold) continue;

        let max_i = std::max_element(edge_weights, edge_weights + palette_len) -
                    edge_weights;

        (*it) = AsciiEl(palette[max_i]);
    }
}
