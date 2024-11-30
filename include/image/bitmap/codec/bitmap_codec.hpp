#ifndef BITMAP_CODEC_HPP
#define BITMAP_CODEC_HPP

#include <exception>

#include "image/bitmap/bitmap.hpp"

class BitmapCodec {
   public:
    // BitmapCodec(std::vector<BitmapCodec> const &codecs) : codecs(codecs) {}
    static Bitmap *decode(char const *const filename);

   private:
    // std::vector<BitmapCodec> const &codecs;

   public:
    struct DecodingException;
    struct UnsupportedFormatException;
};

struct BitmapCodec::DecodingException : public std::exception {
    char const *what() const throw() override {
        return "Generic decoding error.";
    }
};
struct BitmapCodec::UnsupportedFormatException : public std::exception {
    char const *what() const throw() override {
        return "Unsupported image format.";
    }
};

#endif
