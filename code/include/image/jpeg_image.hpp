#ifndef JPEG_IMAGE_HPP
#define JPEG_IMAGE_HPP

#include <cstddef>

#include "dims.hpp"
#include "image/image.hpp"

class JpegImage : public Image {
   public:
    JpegImage(char const *const filename);
    ~JpegImage();

    Size get_size(void) const override { return size; }
    Pixel get_pixel(Pos const pos) const override {
        return ((uint32_t *)data
        )[pos.y * size.w + pos.x];  // Access data in row-major order
    }

   private:
    void read_jpeg_data(uint8_t const *const src_data, size_t const src_size);

    Size size = Size(0, 0);
    void *data;
};

#endif
