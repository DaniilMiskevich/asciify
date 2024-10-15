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

// class BlurEffect : public AsciiEffect {
//    public:
//     BlurEffect(AsciiArt const &image) : AsciiEffect(image) {}
//
//     AsciiArt apply() const override {
//         AsciiArt result(image_.width(), image_.height());
//
//         for (int y = 1; y < image_.height() - 1; ++y) {
//             for (int x = 1; x < image_.width() - 1; ++x) {
//                 int sum = 0;
//                 int count = 0;
//
//                 for (int dy = -1; dy <= 1; ++dy) {
//                     for (int dx = -1; dx <= 1; ++dx) {
//                         if (!image_.at(x + dx, y + dy).transparent) {
//                             sum += int(image_.at(x + dx, y + dy).character);
//                             count++;
//                         }
//                     }
//                 }
//
//                 result.at(x, y) = Asciiel{char(sum / count), false};
//             }
//         }
//
//         return result;
//     }
// };
//
// class CombineEffect : public AsciiEffect {
//    public:
//     CombineEffect(
//         AsciiArt const &image,
//         AsciiEffect const &effect1,
//         AsciiEffect const &effect2
//     )
//     : Effect(image), effect1_(effect1), effect2_(effect2) {}
//
//     AsciiArt apply() const override {
//         AsciiArt result1 = effect1_.apply();
//         AsciiArt result2 = effect2_.apply();
//
//         AsciiArt combined(image_.width(), image_.height());
//
//         for (int y = 0; y < image_.height(); ++y) {
//             for (int x = 0; x < image_.width(); ++x) {
//                 Asciiel const &pixel1 = result1.at(x, y);
//                 Asciiel const &pixel2 = result2.at(x, y);
//
//                 if (!pixel1.transparent) {
//                     combined.at(x, y) = pixel1;
//                 } else if (!pixel2.transparent) {
//                     combined.at(x, y) = pixel2;
//                 }
//             }
//         }
//
//         return combined;
//     }
//
//    private:
//     Effect const &effect1_;
//     Effect const &effect2_;
// };

#endif
