#ifndef WEBP_CODEC_HPP
#define WEBP_CODEC_HPP

#include <cstddef>
#include <exception>

#include "image/bitmap/codec/bitmap_codec.hpp"

class WebpCodec : public BitmapCodec {
   public:
    static Bitmap *decode(uint8_t const *const src_data, size_t const src_size);

    static void encode(Image<Color> const &src, char const *const path);

   public:
    struct InternalEncodingException;
};

struct WebpCodec::InternalEncodingException : public std::exception {
    InternalEncodingException(char const *const msg) : msg(msg) {}

    char const *const msg;

    char const *what() const throw() override { return msg; }
};

#endif
