#ifndef WEBP_IMAGE_HPP
#define WEBP_IMAGE_HPP

#include <cstddef>

#include "image/image.hpp"

struct WebpImage {
    static Image decode(uint8_t const *const src_data, size_t const src_size);
    static void encode(Image const &src, char const *const path);
};

#endif
