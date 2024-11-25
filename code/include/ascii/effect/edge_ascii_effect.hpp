#ifndef EDGE_ASCII_EFFECT_HPP
#define EDGE_ASCII_EFFECT_HPP

#include <cassert>
#include <cstring>

#include "ascii_effect.hpp"

class EdgeAsciiEffect : public AsciiEffect {
   public:
    EdgeAsciiEffect(
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

    void operator()(AsciiArt &dst) const override;

   private:
    float const threshold;
    char const *const palette;
    size_t const palette_len;
    float const dog_eps, dog_p;
};

#endif
