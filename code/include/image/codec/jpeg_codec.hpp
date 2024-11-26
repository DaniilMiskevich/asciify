#ifndef JPEG_CODEC_HPP
#define JPEG_CODEC_HPP

#include <cstddef>

#include "color.hpp"
#include "image/codec/image_codec.hpp"
#include "image/image.hpp"

class JpegCodec : public ImageCodec {
   public:
    static Image<Color>
    decode(uint8_t const *const src_data, size_t const src_size);

   public:
    struct InternalDecodingException;
    struct InvalidHeaderDecodingException;
};

struct JpegCodec::InternalDecodingException :
public ImageCodec::DecodingException {
    InternalDecodingException(char const *const msg) : msg(msg) {}

    char const *const msg;

    char const *what() const throw() override { return msg; }
};

struct JpegCodec::InvalidHeaderDecodingException :
public ImageCodec::DecodingException {
    char const *what() const throw() override { return "Invalid JPEG header."; }
};

#endif
