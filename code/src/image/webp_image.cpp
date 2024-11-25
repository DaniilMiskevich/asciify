// man: https://developers.google.com/speed/webp/docs/api

#include "image/webp_image.hpp"

#include <fstream>
#include <iostream>
#include <new>

#include <webp/decode.h>
#include <webp/encode.h>

WebpImage
WebpImage::decode(uint8_t const *const src_data, size_t const src_size) {
    int width, height;
    let data = reinterpret_cast<Pixel const *>(
        WebPDecodeRGB(src_data, src_size, &width, &height)
    );
    if (data == nullptr) throw std::bad_alloc();

    return WebpImage(data, Size(width, height));
}

void WebpImage::encode(Image const &src, char const *const path) {
    let size = src.get_size();

    let pixels = new Pixel[size.get_area()];
    for (letmut i = size_t(0); i < size.get_area(); i++) {
        let col = src[Pos(i % size.w, i / size.w)];
        // TODO use uint32_t(Color)
        pixels[i][0] = col.r * 0xFF;
        pixels[i][1] = col.g * 0xFF;
        pixels[i][2] = col.b * 0xFF;
    }

    uint8_t *out;
    let out_len = WebPEncodeRGB(
        reinterpret_cast<uint8_t *>(pixels),
        size.w,
        size.h,
        size.w * 3,
        100,
        &out
    );
    // TODO
    assert(out_len != 0);

    {
        letmut file = std::ofstream(path, std::ios::binary);
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        file.write(reinterpret_cast<char *>(out), out_len);
    }

    WebPFree(out);
    delete[] pixels;
}

WebpImage::~WebpImage() { WebPFree(const_cast<Pixel *>(data)); }
