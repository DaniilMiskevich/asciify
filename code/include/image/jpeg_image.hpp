#ifndef JPEG_IMAGE_HPP
#define JPEG_IMAGE_HPP

#include <cstddef>

#include "dims.hpp"
#include "image/image.hpp"

class JpegImage : public Image {
   public:
    static JpegImage
    decode(uint8_t const *const src_data, size_t const src_size);

    JpegImage(JpegImage const &other)
    : _size(other._size), data(new Color[_size.area()]()) {
        std::copy(
            other.data,
            other.data + _size.area(),
            const_cast<Color *>(data)
        );
    }

    ~JpegImage() { delete[] data; }

    Size size() const override { return _size; }

    Color const &operator[](Pos pos) const override {
        if (pos.x >= _size.w) pos.x = _size.w - 1;
        if (pos.y >= _size.h) pos.y = _size.h - 1;
        return data[pos.x + pos.y * _size.w];
    }

   private:
    JpegImage(Color const *const data, Size const size)
    : _size(size), data(data) {}

    Size const _size;
    Color const *const data;

   public:
    class InternalLoadingException;
    class InvalidHeaderLoadingException;
};

class JpegImage::InternalLoadingException : public Image::LoadingException {
   public:
    InternalLoadingException(char const *const msg) : msg(msg) {}

    char const *what() const throw() override { return msg; }

   private:
    char const *const msg;
};

class JpegImage::InvalidHeaderLoadingException :
public Image::LoadingException {
   public:
    char const *what() const throw() override { return "Invalid JPEG header."; }
};

#endif
