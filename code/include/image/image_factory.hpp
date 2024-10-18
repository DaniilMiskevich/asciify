#ifndef IMAGE_FACTORY_HPP
#define IMAGE_FACTORY_HPP

#include <stdexcept>

#include "image/image.hpp"
#include "image/jpeg_image.hpp"
#include "image/webp_image.hpp"

class ImageFactory {
   public:
    ImageFactory() {}

    Image const *create(char const *const filename) const {
        // this is a hacky piece of code, very concise though
        try {
            _create<WebpImage>(filename);
            _create<JpegImage>(filename);
            throw std::runtime_error("Unsupported image format.");
        } catch (Image const *const image) { return image; }
    }

   private:
    template <typename T>
    T const *_create(char const *const filename) const {
        try {
            throw new T(filename);
        } catch (std::exception &e) { return nullptr; }
    }
};

#endif
