#ifndef ASCII_EFFECT_HPP
#define ASCII_EFFECT_HPP

#include "effect/ascii_art.hpp"
#include "image/image.hpp"

struct AsciiEffect {
    virtual void operator()(Image const &src, AsciiArt &tgt) const = 0;
};

#endif
