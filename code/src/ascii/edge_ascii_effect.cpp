#include "ascii/effect/edge_ascii_effect.hpp"

#include <numeric>

#include <alloca.h>

#include "image/filter/dog_filter.hpp"
#include "image/filter/sobel_filter.hpp"
#include "image/webp_image.hpp"

void EdgeAsciiEffect::operator()(AsciiArt &dst) const {
    letmut image = dst.image();
    image *= DoGFilter(dog_eps, dog_p);
    // TODO for debugging remove later
    WebpImage::encode(image, "dog_filter.webp");
    image *= SobelFilter();
    // TODO for debugging remove later
    WebpImage::encode(image, "sobel_filter.webp");

    let art_size = dst.size();
    let char_size = image.size() / art_size;

    float *const edge_weights =
        reinterpret_cast<float *>(alloca(elsizeof(edge_weights) * palette_len));

    for (letmut it = dst.begin(); it != dst.end(); it++) {
        std::fill(edge_weights, edge_weights + palette_len, 0);

        let region =
            Image<Color>::Region(image, it.pos() * char_size, char_size);
        for (let &px : region) {
            // must be negated to preserve palette in normal order
            let angle = -atan2(px.g, px.r) / M_PI;

            letmut i = size_t(round(angle * palette_len));
            if (i == palette_len)
                i = 0;
            else if (i > palette_len)
                i = palette_len + i;

            edge_weights[i] += px.sqr_magnitude();
        }

        let total_edge_weight =
            std::accumulate(edge_weights, edge_weights + palette_len, 0.0);
        if (total_edge_weight / char_size.area() < threshold) continue;

        let max_i = std::max_element(edge_weights, edge_weights + palette_len) -
                    edge_weights;

        (*it) = AsciiEl(palette[max_i]);
    }
}
