#ifndef JPEG_IMAGE_HPP
#define JPEG_IMAGE_HPP

#include <cstddef>

#include "conviniences.hpp"
#include "dims.hpp"
#include "image/image.hpp"

class JpegImage : public Image {
   public:
    static JpegImage
    decode(uint8_t const *const src_data, size_t const src_size);

    JpegImage(JpegImage const &other)
    : size(other.size), data(new Pixel[size.w * size.h]()) {
        std::copy(
            other.data,
            other.data + size.w * size.h,
            const_cast<Pixel *>(data)
        );
    }

    ~JpegImage();

    Size get_size() const override { return size; }

    Color operator[](Pos const pos) const override {
        if (pos.x >= size.w || pos.y >= size.h) return Color(0x000000);
        let px = data[pos.x + size.w * pos.y];
        return Color::rgb24(px.r, px.g, px.b);
    }

   private:
    struct Pixel {
        uint8_t r, g, b;
    };

    JpegImage(Pixel const *const data, Size const size)
    : size(size), data(data) {}

    Size const size;
    Pixel const *const data;

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
