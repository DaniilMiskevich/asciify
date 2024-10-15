#ifndef WEBP_IMAGE_HPP
#define WEBP_IMAGE_HPP

#include "image/image.hpp"

class WebpImage : public Image {
public:
  WebpImage(char const *const filename);
  ~WebpImage();

  Size get_size(void) const override { return size; }
  Pixel get_pixel(Pos const pos) const override {
    return data[pos.y + size.h * pos.x];
  }

private:
  uint32_t *data;
  Size size = Size(0, 0);
};

#endif
