// man: http://www.libpng.org/pub/png/libpng-manual.txt

#include "image/webp_image.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include <webp/decode.h>

#include "conviniences.hpp"

WebpImage::WebpImage(char const *const filename) {
    letmut source = std::ifstream(filename, std::ios::binary);
    source.seekg(0, std::ios::end);
    let source_size = source.tellg();
    source.seekg(0, std::ios::beg);

    letmut data = std::vector<uint8_t>(source_size);
    for (letmut data_iter = data.begin(); source >> *data_iter; data_iter++);

    int width = 0, height = 0;
    if (!WebPGetInfo(data.data(), data.size(), &width, &height)) {
        letmut features = WebPBitstreamFeatures();
        std::cout << WebPGetFeatures(data.data(), data.size(), &features)
                  << "\n";
        VP8StatusCode::VP8_STATUS_INVALID_PARAM;
        throw "TODO SomeInternalImageReadingError";
    }

    size = Size(width, height);
}

Image::Pixel WebpImage::get_pixel(size_t const x, size_t const y) const {
    return x + y;
}
