#ifndef ASCII_EFFECT_HPP
#define ASCII_EFFECT_HPP

#include "ascii/ascii_art.hpp"
#include "image/filter/filter.hpp"

struct AsciiEffect : public Filter<AsciiEl, AsciiArt> {
    virtual void operator()(AsciiArt &dst) const = 0;
};

#endif
