#ifndef WEBP_IMAGE_HPP
#define WEBP_IMAGE_HPP

#include <cstddef>

#include "conviniences.hpp"
#include "image/image.hpp"

class WebpImage : public Image {
   public:
    static WebpImage
    decode(uint8_t const *const src_data, size_t const src_size);

    static void encode(Image const &src, char const *const path);

    WebpImage(WebpImage const &other)
    : size(other.size), data(new Pixel[size.w * size.h]()) {
        std::copy(
            other.data,
            other.data + size.w * size.h,
            const_cast<Pixel *>(data)
        );
    }

    ~WebpImage();

    Size get_size() const override { return size; }

    Color operator[](Pos const pos) const override {
        if (pos.x >= size.w || pos.y >= size.h) return Color(0x000000);
        let px = data[pos.x + size.w * pos.y];
        return Color::rgb24(px.r, px.g, px.b);
    }

   private:
    struct Pixel {
        uint8_t r, g, b;
    };

    WebpImage(Pixel const *const data, Size const size)
    : size(size), data(data) {}

    Size const size;
    Pixel const *const data;
};

#endif
