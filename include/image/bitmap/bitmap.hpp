#ifndef BITMAP_HPP
#define BITMAP_HPP

#include "image/color.hpp"
#include "image/image.hpp"

class Bitmap : public Image<Color> {
   public:
    explicit Bitmap(Size const size) : Image<Color>(size) {}
};

#endif
