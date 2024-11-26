#ifndef ASCII_ART_HPP
#define ASCII_ART_HPP

#include "ascii/ascii_el.hpp"
#include "dims.hpp"
#include "image/image.hpp"

class AsciiArt : public Image<AsciiEl> {
   public:
    AsciiArt(Image<Color> const &image, Size const char_size)
    : Image<AsciiEl>(image.size() / char_size), _image(image) {}

    Image<Color> const &image() const { return _image; }

   private:
    Image<Color> const &_image;
};

#endif
