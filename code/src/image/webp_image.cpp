// man: https://developers.google.com/speed/webp/docs/api

#include "image/webp_image.hpp"

#include <stdexcept>

#include <webp/decode.h>

WebpImage::WebpImage(uint8_t const *const src_data, size_t const src_size) {
    int width, height;
    data = reinterpret_cast<WebpPixel const *>(
        WebPDecodeRGB(src_data, src_size, &width, &height)
    );
    if (data == nullptr) throw std::runtime_error("WebP decoding failed!");

    size = Size(width, height);
}

WebpImage::~WebpImage() { WebPFree(const_cast<WebpPixel *>(data)); }
