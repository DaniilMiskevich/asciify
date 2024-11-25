#ifndef WEBP_IMAGE_HPP
#define WEBP_IMAGE_HPP

#include <cstddef>

#include "image/image.hpp"

class WebpImage : public Image {
   public:
    static WebpImage
    decode(uint8_t const *const src_data, size_t const src_size);

    static void encode(Image const &src, char const *const path);

    WebpImage(WebpImage const &other)
    : _size(other._size), data(new Color[_size.area()]()) {
        std::copy(
            other.data,
            other.data + _size.area(),
            const_cast<Color *>(data)
        );
    }

    ~WebpImage() { delete[] data; }

    Size size() const override { return _size; }

    Color const &operator[](Pos pos) const override {
        if (pos.x >= _size.w) pos.x = _size.w - 1;
        if (pos.y >= _size.h) pos.y = _size.h - 1;
        return data[pos.x + pos.y * _size.w];
    }

   private:
    WebpImage(Color const *const data, Size const size)
    : _size(size), data(data) {}

    Size const _size;
    Color const *const data;
};

#endif
