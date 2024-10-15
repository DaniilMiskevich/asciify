#ifndef ASCII_EFFECT_HPP
#define ASCII_EFFECT_HPP

#include <cmath>

#include "conviniences.hpp"
#include "dims.hpp"
#include "effect/ascii_art.hpp"
#include "image/image.hpp"

struct AsciiEffect {
  virtual void operator()(Image const &src, AsciiArt &tgt) const = 0;
};

struct LuminanceAsciiEffect : private AsciiEffect {
  void operator()(Image const &src, AsciiArt &tgt) const override {
    for (letmut i = uint16_t(0); i < tgt.get_size().w; i++) {
      for (letmut j = uint16_t(0); j < tgt.get_size().h; j++) {
        letmut avg_luminance = 0.0;
        for (letmut dx = 0; dx < 16; dx++)
          for (letmut dy = 0; dy < 8; dy++) {
            let pix = src.get_pixel(Pos(i * 16 + dx, j * 8 + dy));
            avg_luminance +=
                (0.2126 * pix.r + 0.7152 * pix.g + 0.0722 * pix.b) /
                (8.0 * 16.0);
          }
        avg_luminance /= 255.0;

        static char chars[10] = {' ', '.', ':', '-', '=',
                                 '+', '*', '#', '%', '@'};
        let quantized_luminance =
            avg_luminance == 1.0 ? 10 : size_t(avg_luminance * 10);

        tgt.set_el(Pos(i, j), AsciiArt::El{chars[quantized_luminance], false});
      }
    }
  }
};

#endif
