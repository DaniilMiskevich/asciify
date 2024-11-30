#ifndef JPEG_CODEC_HPP
#define JPEG_CODEC_HPP

#include <cstddef>

#include "image/bitmap/codec/bitmap_codec.hpp"

class JpegCodec : public BitmapCodec {
   public:
    static Bitmap *decode(uint8_t const *const src_data, size_t const src_size);

   public:
    struct InternalDecodingException;
    struct InvalidHeaderDecodingException;
};

struct JpegCodec::InternalDecodingException :
public JpegCodec::DecodingException {
    InternalDecodingException(char const *const msg) : msg(msg) {}

    char const *const msg;

    char const *what() const throw() override { return msg; }
};

struct JpegCodec::InvalidHeaderDecodingException :
public JpegCodec::DecodingException {
    char const *what() const throw() override { return "Invalid JPEG header."; }
};

#endif
