#ifndef ASCII_EFFECT_HPP
#define ASCII_EFFECT_HPP

#include "ascii/ascii_art.hpp"

class AsciiEffect {
   public:
    virtual void operator()(AsciiArt &dst) const = 0;
};

#endif
