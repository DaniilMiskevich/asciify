#include <sstream>

#include "conviniences.hpp"
#include "font.hpp"
#include "image/ascii/ascii_writer.hpp"
#include "image/ascii/filter/color_ascii_filter.hpp"
#include "image/ascii/filter/edge_ascii_filter.hpp"
#include "image/ascii/filter/fill_ascii_filter.hpp"
#include "image/bitmap/codec/bitmap_codec.hpp"

extern "C" {

// fill
FillAsciiFilter *fill_filter_create(char const *const palette) {
    return new FillAsciiFilter(palette);
}
void fill_filter_delete(FillAsciiFilter const *const self) { delete self; }

// color
ColorAsciiFilter *color_filter_create() { return new ColorAsciiFilter(); }
void color_filter_delete(ColorAsciiFilter const *const self) { delete self; }

// edge
EdgeAsciiFilter *
edge_filter_create(float const threshold, char const *const palette) {
    return new EdgeAsciiFilter(threshold, palette);
}
EdgeAsciiFilter *edge_filter_create_extra(
    float const threshold,
    char const *const palette,
    float const dog_eps,
    float const dog_p
) {
    return new EdgeAsciiFilter(threshold, palette, dog_eps, dog_p);
}
void edge_filter_delete(EdgeAsciiFilter const *const self) { delete self; }

// bitmap
Bitmap *bitmap_decode(char const *const path) {
    try {
        return BitmapCodec::decode(path);
    } catch (std::exception const &) { return nullptr; }
}
void bitmap_delete(Bitmap const *const self) { delete self; }

// ascii

Ascii *ascii_create(
    Bitmap const *const bitmap,
    Size const frame_size_chars,
    char const *const font_family,
    float const font_size
) {
    letmut char_size_px = Size(10, 20);
    try {
        if (font_family != nullptr)
            char_size_px = Font::load(font_family, font_size).size();
    } catch (std::exception const &) {
        // use default size
    }
    return new Ascii(*bitmap, frame_size_chars, char_size_px);
}
void ascii_delete(Ascii const *const self) { delete self; }

struct AsciiOutput {
    char const *const cstr;
    size_t const len;
};
AsciiOutput ascii_write(
    Ascii const *const self,
    Size const frame_size,
    AsciiWriter::ColorMode const mode
) {
    let writer = AsciiWriter(*self, frame_size);

    letmut stream = std::stringstream();
    writer.write_to(stream, mode);

    let string = stream.str();
    let len = string.length();

    let cstr = new char[len]();
    std::move(string.begin(), string.end(), cstr);
    return AsciiOutput{cstr, len};
}

void ascii_apply_fill(Ascii *const self, FillAsciiFilter const *const fill) {
    *self *= *fill;
}
void ascii_apply_color(Ascii *const self, ColorAsciiFilter const *const color) {
    *self *= *color;
}
void ascii_apply_edge(Ascii *const self, EdgeAsciiFilter const *const edge) {
    *self *= *edge;
}
}
