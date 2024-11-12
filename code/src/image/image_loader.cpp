#include "image/image_loader.hpp"

#include <fstream>
#include <iostream>

#include "image/jpeg_image.hpp"
#include "image/webp_image.hpp"

template <typename T>
static void pass_image(uint8_t const *const src_data, size_t const src_size) {
    try {
        throw new T(T::load(src_data, src_size));
    } catch (std::exception &e) {}
}

Image const *ImageLoader::load(char const *const filename) const {
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

    Image const *image = nullptr;
    try {
        pass_image<WebpImage>(src_data, src_size);
        pass_image<JpegImage>(src_data, src_size);
    } catch (Image const *passed_image) { image = passed_image; }

    delete[] src_data;

    if (image == nullptr) throw UnsupportedFormatException();

    return image;
}
