#ifndef ASCII_HPP
#define ASCII_HPP

#include <algorithm>

#include "image/bitmap/bitmap.hpp"
#include "image/color.hpp"
#include "image/image.hpp"

struct AsciiEl {
    AsciiEl() : AsciiEl(' ') {}

    explicit AsciiEl(char const c, Color const &color = Color(0xFFFFFF))
    : c(c), color(color) {}

    char c;
    Color color;
};

class Ascii : public Image<AsciiEl> {
   public:
    Ascii(
        Bitmap const &bitmap,
        Size const frame_size_chars,
        Size const char_size
    )
    : Image<AsciiEl>(
          bitmap.size() /
          (fit_char_size(char_size, bitmap.size(), frame_size_chars))
      ),
      _char_size(fit_char_size(char_size, bitmap.size(), frame_size_chars)),
      _bitmap(bitmap) {}

    Size char_size() const { return _char_size; }
    Bitmap const &bitmap() const { return _bitmap; }

   private:
    Size const _char_size;
    Bitmap const &_bitmap;

    constexpr static float
    fit_ratio(Size const image_size_px, Size const frame_size_px) {
        return std::max(
            float(image_size_px.w) / frame_size_px.w,
            float(image_size_px.h) / frame_size_px.h
        );
    }
    constexpr static Size fit_char_size(
        Size const char_size,
        Size const image_size_px,
        Size const frame_size_chars
    ) {
        return char_size *
                   fit_ratio(image_size_px, frame_size_chars * char_size)
             // allows chars to be a bit bigger == image will contain less
             // chars, never more
             + Size(1, 1);
    }
};

#endif
