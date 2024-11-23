#ifndef WEBP_IMAGE_HPP
#define WEBP_IMAGE_HPP

#include <cstddef>

#include "conviniences.hpp"
#include "image/image.hpp"

class WebpImage : public Image {
   public:
    static WebpImage
    decode(uint8_t const *const src_data, size_t const src_size);

    ~WebpImage();

    Size get_size() const override { return size; }

    Color operator[](Pos const pos) const override {
        if (pos.x >= size.w || pos.y >= size.h) return Color::hex(0x000000);

        let px = data[pos.x + size.w * pos.y];
        return Color::rgb255(px[0], px[1], px[2]);
    }

   private:
    typedef uint8_t WebpPixel[3];

    WebpImage(WebpPixel const *const data, Size const size)
    : data(data), size(size) {}

    WebpPixel const *const data;
    Size const size;
};

#endif
