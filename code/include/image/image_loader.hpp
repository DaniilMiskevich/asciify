#ifndef IMAGE_LOADER_HPP
#define IMAGE_LOADER_HPP

#include <exception>

#include "color.hpp"
#include "image/image.hpp"

class ImageLoader {
   public:
    Image<Color> decode(char const *const filename) const;

   public:
    class UnsupportedFormatException;
};

class ImageLoader::UnsupportedFormatException : public std::exception {
   public:
    char const *what() const throw() override {
        return "Unsupported image format.";
    }
};

#endif
