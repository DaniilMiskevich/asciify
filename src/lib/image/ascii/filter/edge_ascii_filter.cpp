#include "image/ascii/filter/edge_ascii_filter.hpp"

#include <numeric>

#include <alloca.h>

#include "image/bitmap/codec/webp_codec.hpp"
#include "image/bitmap/filter/dog_bitmap_filter.hpp"
#include "image/bitmap/filter/sobel_bitmap_filter.hpp"

void EdgeAsciiFilter::operator()(Ascii &dst) const {
    letmut bitmap = Bitmap(dst.bitmap());
    bitmap *= DoGBitmapFilter(dog_eps, dog_p);
    WebpCodec::encode(bitmap, "dog_filter.webp");
    bitmap *= SobelBitmapFilter();
    WebpCodec::encode(bitmap, "sobel_filter.webp");

    let char_size = dst.char_size();

    float *const edge_weights =
        reinterpret_cast<float *>(alloca(elsizeof(edge_weights) * palette_len));

    for (letmut it = dst.begin(); it != dst.end(); it++) {
        std::fill(edge_weights, edge_weights + palette_len, 0);

        let region = Bitmap::Region(bitmap, it.pos() * char_size, char_size);
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
        *it = AsciiEl(palette[max_i]);
    }
}
