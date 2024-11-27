#include <sstream>

#include "ascii/ascii_art.hpp"
#include "ascii/ascii_art_writer.hpp"
#include "ascii/filter/color_ascii_filter.hpp"
#include "ascii/filter/edge_ascii_filter.hpp"
#include "ascii/filter/fill_ascii_filter.hpp"
#include "conviniences.hpp"
#include "image/codec/image_codec.hpp"

extern "C" {

// fill
FillAsciiFilter const *fill_filter_create(char const *const palette) {
    return new FillAsciiFilter(palette);
}
void fill_filter_delete(FillAsciiFilter const *const self) { delete self; }

// color
ColorAsciiFilter const *color_filter_create() { return new ColorAsciiFilter(); }
void color_filter_delete(ColorAsciiFilter const *const self) { delete self; }

// edge
EdgeAsciiFilter const *
edge_filter_create(float const threshold, char const *const palette) {
    return new EdgeAsciiFilter(threshold, palette);
}
EdgeAsciiFilter const *edge_filter_create_extra(
    float const threshold,
    char const *const palette,
    float const dog_eps,
    float const dog_p
) {
    return new EdgeAsciiFilter(threshold, palette, dog_eps, dog_p);
}
void edge_filter_delete(EdgeAsciiFilter const *const self) { delete self; }

// image
Image<Color> const *image_decode(char const *const path) {
    try {
        return ImageCodec::decode(path);
    } catch (ImageCodec::UnsupportedFormatException const &) { return nullptr; }
}
void image_delete(Image<Color> const *const self) { delete self; }

// ascii art

AsciiArt *
ascii_art_create(Image<Color> const *const image, Size const char_size) {
    return new AsciiArt(*image, char_size);
}
void ascii_art_delete(AsciiArt const *const self) { delete self; }

struct AsciiArtOutput {
    char const *const cstr;
    size_t const len;
};
AsciiArtOutput ascii_art_write(
    AsciiArt const *const self,
    AsciiArtWriter::ColorMode const mode
) {
    let writer = AsciiArtWriter(*self);

    letmut stream = std::stringstream();
    writer.write_to(stream, mode);

    let string = stream.str();
    let len = string.length();

    let cstr = new char[len]();
    std::move(string.begin(), string.end(), cstr);
    return AsciiArtOutput{cstr, len};
}

void ascii_art_apply_fill(
    AsciiArt *const self,
    FillAsciiFilter const *const fill
) {
    *self *= *fill;
}
void ascii_art_apply_color(
    AsciiArt *const self,
    ColorAsciiFilter const *const color
) {
    *self *= *color;
}
void ascii_art_apply_edge(
    AsciiArt *const self,
    EdgeAsciiFilter const *const edge
) {
    *self *= *edge;
}
}
