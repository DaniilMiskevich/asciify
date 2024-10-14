#ifndef WEBP_IMAGE_HPP
#define WEBP_IMAGE_HPP

#include "image/image.hpp"

class WebpImage : private Image {
   public:
    WebpImage(char const *const filename);

    Size get_size(void) const { return size; }
    Pixel get_pixel(size_t const x, size_t const y) const;

   private:
    Size size = Size(0, 0);
};

#endif
