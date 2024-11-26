#ifndef JPEG_IMAGE_HPP
#define JPEG_IMAGE_HPP

#include <cstddef>

#include "color.hpp"
#include "image/image.hpp"

struct JpegImage {
    static Image<Color>
    decode(uint8_t const *const src_data, size_t const src_size);

   public:
    class InternalLoadingException;
    class InvalidHeaderLoadingException;
};

class JpegImage::InternalLoadingException :
public LegacyImage::LoadingException {
   public:
    InternalLoadingException(char const *const msg) : msg(msg) {}

    char const *what() const throw() override { return msg; }

   private:
    char const *const msg;
};

class JpegImage::InvalidHeaderLoadingException :
public LegacyImage::LoadingException {
   public:
    char const *what() const throw() override { return "Invalid JPEG header."; }
};

#endif
