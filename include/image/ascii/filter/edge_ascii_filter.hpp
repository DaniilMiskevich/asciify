#ifndef EDGE_ASCII_FILTER_HPP
#define EDGE_ASCII_FILTER_HPP

#include <cstring>

#include "image/ascii/ascii.hpp"
#include "image/ascii/filter/ascii_filter.hpp"

class EdgeAsciiFilter : public AsciiFilter {
   public:
    explicit EdgeAsciiFilter(
        float const threshold,
        char const *const palette = "|\\-/",
        float const dog_eps = 1,
        float const dog_p = 10
    )
    : threshold(threshold),
      palette(palette),
      palette_len(strlen(palette)),
      dog_eps(dog_eps),
      dog_p(dog_p) {}

   private:
    float const threshold;
    char const *const palette;
    size_t const palette_len;
    float const dog_eps, dog_p;

    void operator()(Ascii &dst) const override;
};

#endif
