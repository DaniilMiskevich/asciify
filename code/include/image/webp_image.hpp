#ifndef WEBP_IMAGE_HPP
#define WEBP_IMAGE_HPP

#include <cstddef>

#include "conviniences.hpp"
#include "image/image.hpp"

class WebpImage : public Image {
   public:
    WebpImage(uint8_t const *const src_data, size_t const src_size);
    ~WebpImage();

    Size get_size() const override { return size; }
    Pixel operator[](Pos const pos) const override {
        if (pos.x >= size.w || pos.y >= size.h) return 0;

        let px = data[pos.x + size.w * pos.y];
        return Pixel(px[0], px[1], px[2]);
    }

   private:
    typedef uint8_t WebpPixel[3];

    WebpPixel const *data;
    Size size = Size(0, 0);
};

#endif
