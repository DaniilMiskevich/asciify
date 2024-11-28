#ifndef WEBP_CODEC_HPP
#define WEBP_CODEC_HPP

#include <cstddef>
#include <exception>

#include "color.hpp"
#include "image/codec/image_codec.hpp"
#include "image/image.hpp"

class WebpCodec : public ImageCodec {
   public:
    static Image<Color> *
    decode(uint8_t const *const src_data, size_t const src_size);

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
