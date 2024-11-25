// man: https://developers.google.com/speed/webp/docs/api

#include "image/webp_image.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <new>

#include <webp/decode.h>
#include <webp/encode.h>

struct Pixel {
    uint8_t r, g, b;
};

WebpImage
WebpImage::decode(uint8_t const *const src_data, size_t const src_size) {
    int width, height;
    let buf = reinterpret_cast<Pixel *>(
        WebPDecodeRGB(src_data, src_size, &width, &height)
    );
    if (buf == nullptr) throw std::bad_alloc();

    let data = new Color[width * height]();
    std::transform(buf, buf + width * height, data, [](Pixel const px) {
        return Color::rgb24(px.r, px.g, px.b);
    });

    WebPFree(buf);

    return WebpImage(data, Size(width, height));
}

void WebpImage::encode(Image const &src, char const *const path) {
    let size = src.size();

    let pixels = new Pixel[size.area()]();
    for (letmut i = size_t(0); i < size.area(); i++) {
        let col = src[Pos(i % size.w, i / size.w)];
        pixels[i].r = col.r8(), pixels[i].g = col.g8(), pixels[i].b = col.b8();
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
