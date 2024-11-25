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

    ~WebpImage();
    // TODO copy + move

    Size get_size() const override { return size; }

    Color operator[](Pos const pos) const override {
        if (pos.x >= size.w || pos.y >= size.h) return Color(0x000000);
        let px = data[pos.x + size.w * pos.y];
        return Color::rgb24(px[0], px[1], px[2]);
    }

   private:
    typedef uint8_t Pixel[3];

    WebpImage(Pixel const *const data, Size const size)
    : data(data), size(size) {}

    Pixel const *const data;
    Size const size;
};

#endif
