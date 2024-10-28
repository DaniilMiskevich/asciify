#ifndef ASCII_EFFECT_HPP
#define ASCII_EFFECT_HPP

#include "ascii_art.hpp"
#include "image/image.hpp"

class AsciiEffect {
    friend class BasicAsciiEffect;

   public:
    AsciiEffect(AsciiEffect const &prev);

    virtual Size get_size() const = 0;
    virtual AsciiArt::El operator[](Pos const pos) const = 0;

   protected:
    AsciiEffect const *const prev;

    virtual Image const &get_src() const = 0;

   private:
    AsciiEffect() : prev(nullptr) {}
};

class BasicAsciiEffect : public AsciiEffect {
   public:
    BasicAsciiEffect(Image const &src, Size const char_size)
    : src(src), char_size(char_size) {}

    Size get_size() const override { return src.get_size() / char_size; }
    AsciiArt::El operator[](Pos const pos) const override {
        return void(pos), AsciiArt::El();
    }

   protected:
    Image const &get_src() const override { return src; }

   private:
    Image const &src;
    Size const char_size;
};

#endif
