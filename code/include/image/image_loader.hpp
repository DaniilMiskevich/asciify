#ifndef IMAGE_LOADER_HPP
#define IMAGE_LOADER_HPP

#include <exception>

#include "image/image.hpp"

class ImageLoader {
   public:
    class UnsupportedFormatException;

    Image const *load(char const *const filename) const;
};

class ImageLoader::UnsupportedFormatException : public std::exception {
   public:
    char const *what() const throw() override {
        return "Unsupported image format.";
    }
};

#endif
