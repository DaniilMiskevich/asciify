#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstddef>
#include <cstdint>

// static_assert(sizeof(Pixel) == 3 * sizeof(Pixel::Channel));

class Image {
   public:
    struct Pixel {
        Pixel() = delete;
        Pixel(uint8_t const r, uint8_t const g, uint8_t const b)
        : r(r), g(g), b(b) {}
        Pixel(uint32_t const hex)
        // dont have to be &'d with 255 coz got truncated during storing into u8
        : r(hex), g(hex >> 8), b(hex >> 16) {}

        uint8_t const r, g, b;
    };

    struct Size {
        Size() = delete;
        Size(unsigned short w, unsigned short h) : w(w), h(h) {}

        unsigned short w, h;
    };

    virtual Size get_size(void) const = 0;
    virtual Pixel get_pixel(size_t const x, size_t const y) const = 0;
};

#endif
