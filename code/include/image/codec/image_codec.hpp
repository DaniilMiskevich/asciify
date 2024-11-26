#ifndef IMAGE_CODEC_HPP
#define IMAGE_CODEC_HPP

#include <exception>

#include "color.hpp"
#include "image/image.hpp"

class ImageCodec {
   public:
    // ImageCodec(std::vector<ImageCodec> const &codecs) : codecs(codecs) {}
    static Image<Color> *decode(char const *const filename);

   private:
    // std::vector<ImageCodec> const &codecs;

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
