#ifndef IMAGE_CODEC_HPP
#define IMAGE_CODEC_HPP

#include <exception>

#include "color.hpp"
#include "image/image.hpp"

class ImageCodec {
   public:
    Image<Color> *decode(char const *const filename) const;

   public:
    struct DecodingException;
    struct UnsupportedFormatException;
};

struct ImageCodec::DecodingException : public std::exception {
    char const *what() const throw() override {
        return "Generic decoding error.";
    }
};
struct ImageCodec::UnsupportedFormatException : public std::exception {
    char const *what() const throw() override {
        return "Unsupported image format.";
    }
};

#endif
