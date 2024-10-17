// man: https://developers.google.com/speed/webp/docs/api

#include "image/webp_image.hpp"

#include <fstream>
#include <iostream>

#include <webp/decode.h>

#include "conviniences.hpp"

WebpImage::WebpImage(char const *const filename) {
    static_assert(
        sizeof(char) == sizeof(uint8_t),
        "This code is expected to work on 1-byte chars only."
    );

    letmut src = std::ifstream(filename, std::ios::binary);
    src.seekg(0, std::ios::end);
    let src_size = src.tellg();
    src.seekg(0, std::ios::beg);

    let src_data = new uint8_t[src_size];
    if (src_data == nullptr) throw std::runtime_error("Allocatoin failed!");
    src.read(reinterpret_cast<char *>(src_data), src_size);

    int width, height;
    data = reinterpret_cast<WebpPixel const *>(
        WebPDecodeRGB(src_data, src_size, &width, &height)
    );
    if (data == nullptr)
        delete[] src_data, throw std::runtime_error("WebP decoding failed!");

    size = Size(width, height);

    delete[] src_data;
}

WebpImage::~WebpImage() { WebPFree(const_cast<WebpPixel *>(data)); }
