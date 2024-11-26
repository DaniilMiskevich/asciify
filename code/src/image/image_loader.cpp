#include "image/image_loader.hpp"

#include <fstream>
#include <iostream>

#include "image/jpeg_image.hpp"
#include "image/webp_image.hpp"

template <typename T>
static void pass_image(uint8_t const *const src_data, size_t const src_size) {
    try {
        throw T::decode(src_data, src_size);
    } catch (std::exception &e) {}
}

Image<Color> ImageLoader::decode(char const *const filename) const {
    static_assert(
        sizeof(char) == sizeof(uint8_t),
        "This code is expected to work on 1-byte chars only."
    );

    letmut src = std::ifstream(filename, std::ios::binary);
    src.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    src.seekg(0, std::ios::end);
    let src_size = src.tellg();
    src.seekg(0, std::ios::beg);

    let src_data = new uint8_t[src_size];
    src.read(reinterpret_cast<char *>(src_data), src_size);

    let decoders = {WebpImage::decode, JpegImage::decode};
    for (let &decode : decoders) {
        try {
            let image = decode(src_data, src_size);

            delete[] src_data;
            return image;
        } catch (std::exception const &e) { continue; }
    }

    delete[] src_data;
    throw UnsupportedFormatException();
}
