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

Image WebpImage::decode(uint8_t const *const src_data, size_t const src_size) {
    int width, height;
    let buf = reinterpret_cast<Pixel *>(
        WebPDecodeRGB(src_data, src_size, &width, &height)
    );
    if (buf == nullptr) throw std::bad_alloc();

    let size = Size(width, height);
    letmut image = Image(size);

    std::transform(buf, buf + size.area(), image.begin(), [](Pixel const px) {
        return Color::rgb24(px.r, px.g, px.b);
    });

    WebPFree(buf);

    return image;
}

void WebpImage::encode(Image const &src, char const *const path) {
    let size = src.size();

    let pixels = new Pixel[size.area()]();
    std::transform(src.begin(), src.end(), pixels, [](Color const col) {
        return Pixel{col.r8(), col.g8(), col.b8()};
    });

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
