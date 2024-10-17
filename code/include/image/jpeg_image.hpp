#ifndef JPEG_IMAGE_HPP
#define JPEG_IMAGE_HPP

#include <cstddef>

#include "conviniences.hpp"
#include "dims.hpp"
#include "image/image.hpp"

class JpegImage : public Image {
   public:
    JpegImage(char const *const filename);
    ~JpegImage();

    Size get_size(void) const override { return size; }
    Pixel get_pixel(Pos const pos) const override {
        if (pos.x >= size.w || pos.y >= size.h) return 0;

        let pix = data[pos.x + size.w * pos.y];
        return Pixel(pix[0], pix[1], pix[2]);
    }

   private:
    typedef uint8_t JpegPixel[3];

    void read_jpeg_data(uint8_t const *const src_data, size_t const src_size);

    JpegPixel const *data;
    Size size = Size(0, 0);
};

#endif
