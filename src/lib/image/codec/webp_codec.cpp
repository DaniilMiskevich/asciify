// man: https://developers.google.com/speed/webp/docs/api

#include "image/bitmap/codec/webp_codec.hpp"

#include <algorithm>
#include <fstream>
#include <new>

#include <webp/decode.h>
#include <webp/encode.h>

struct RGBAPixel {
    uint8_t r, g, b, a;
};

Bitmap *
WebpCodec::decode(uint8_t const *const src_data, size_t const src_size) {
    int width, height;
    let buf = reinterpret_cast<RGBAPixel *>(
        WebPDecodeRGBA(src_data, src_size, &width, &height)
    );
    if (buf == nullptr) throw std::bad_alloc();

    let size = Size(width, height);
    letmut bitmap = new Bitmap(size);

    std::transform(
        buf,
        buf + size.area(),
        bitmap->begin(),
        [](RGBAPixel const px) {
            return Color::rgb24(px.r, px.g, px.b) * px.a / 0xFF;
        }
    );

    WebPFree(buf);

    return bitmap;
}

void WebpCodec::encode(Image<Color> const &src, char const *const path) {
    let size = src.size();

    let pixels = new RGBAPixel[size.area()]();
    std::transform(src.begin(), src.end(), pixels, [](Color const col) {
        return RGBAPixel{col.r8(), col.g8(), col.b8(), 0xFF};
    });

    uint8_t *out;
    let out_len = WebPEncodeRGBA(
        reinterpret_cast<uint8_t *>(pixels),
        size.w,
        size.h,
        size.w * elsizeof(pixels),
        100,
        &out
    );
    if (out_len == 0)
        throw InternalEncodingException("Internal encoding exception.");

    {
        letmut file = std::ofstream(path, std::ios::binary);
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        file.write(reinterpret_cast<char *>(out), out_len);
    }

    WebPFree(out);

    delete[] pixels;
}
