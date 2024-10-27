#ifndef IMAGE_FACTORY_HPP
#define IMAGE_FACTORY_HPP

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "image/image.hpp"
#include "image/jpeg_image.hpp"
#include "image/webp_image.hpp"

class ImageLoader {
   public:
    ImageLoader() {}

    Image const *load(char const *const filename) const {
        static_assert(
            sizeof(char) == sizeof(uint8_t),
            "This code is expected to work on 1-byte chars only."
        );

        letmut src = std::ifstream(filename, std::ios::binary);
        src.seekg(0, std::ios::end);
        let src_size = src.tellg();
        src.seekg(0, std::ios::beg);

        let src_data = new uint8_t[src_size];
        src.read(reinterpret_cast<char *>(src_data), src_size);

        // this is a hacky piece of code, very concise though
        try {
            pass_image<WebpImage>(src_data, src_size);
            pass_image<JpegImage>(src_data, src_size);

            delete[] src_data;
            throw std::runtime_error("Unsupported image format.");
        } catch (Image const *const image) {
            delete[] src_data;
            return image;
        }
    }

   private:
    template <typename T>
    T const *pass_image(uint8_t const *const src_data, size_t const src_size)
        const {
        try {
            throw new T(src_data, src_size);
        } catch (std::exception &e) { return nullptr; }
    }
};

#endif
