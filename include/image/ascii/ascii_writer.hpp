#ifndef ASCII_WRITER_HPP
#define ASCII_WRITER_HPP

#include <fstream>

#include "image/ascii/ascii.hpp"

class AsciiWriter {
   public:
    enum ColorMode { COLOR_MODE_NONE, COLOR_MODE_INDEXED, COLOR_MODE_TRUE };

    AsciiWriter(Ascii const &art, Size const frame_size)
    : frame_size(frame_size), art(art) {}

    void write_to(
        std::ostream &stream,
        ColorMode const color_mode = COLOR_MODE_INDEXED
    ) const {
        let size = art.size();
        let dw = frame_size.w - size.w, dh = frame_size.h - size.h;

        for (letmut i = int(0); i < dh / 2; i++) stream << std::endl;

        letmut pos = Pos(0, 0);
        for (pos.y = 0; pos.y < size.h; pos.y++) {
            for (letmut i = int(0); i < dw / 2; i++) stream << ' ';

            for (pos.x = 0; pos.x < size.w; pos.x++) {
                let el = art[pos];

                switch (color_mode) {
                case COLOR_MODE_NONE: stream << el.c; break;

                case COLOR_MODE_INDEXED: {
                    stream << "\033[3" << indexed_color_index(el.color) << ";2m"

                           << el.c

                           << "\033[0m";
                } break;

                case COLOR_MODE_TRUE: {
                    stream << "\033[38;2;"

                           << int(el.color.r8()) << ";" << int(el.color.g8())
                           << ";" << int(el.color.b8()) << "m"

                           << el.c

                           << "\033[0m";
                } break;
                }
            }

            for (letmut i = int(0); i < dw - dw / 2; i++) stream << ' ';
            stream << std::endl;
        }

        for (letmut i = int(0); i < dh - dh / 2 - 1; i++) stream << std::endl;
    }

    void write_to_file(
        char const *const filename,
        ColorMode const color_mode = COLOR_MODE_NONE
    ) const {
        letmut out = std::ofstream(filename);
        out.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        write_to(out, color_mode);
    }

   private:
    static constexpr Color const indexed_colors[] = {
#ifdef _MSC_BUILD  // windows console colors
        Color::rgb255(12, 12, 12),
        Color::rgb255(197, 15, 31),
        Color::rgb255(19, 161, 14),
        Color::rgb255(193, 156, 0),
        Color::rgb255(0, 55, 218),
        Color::rgb255(136, 23, 152),
        Color::rgb255(58, 150, 221),
        Color::rgb255(204, 204, 204),
#else  // xterm colors
        Color::rgb24(0, 0, 0),
        Color::rgb24(205, 0, 0),
        Color::rgb24(0, 205, 0),
        Color::rgb24(205, 205, 0),
        Color::rgb24(0, 0, 238),
        Color::rgb24(205, 0, 205),
        Color::rgb24(0, 205, 205),
        Color::rgb24(229, 229, 229),
#endif
    };

    Size const frame_size;
    Ascii const &art;

    constexpr unsigned indexed_color_index(Color const &color) const {
        return std::min_element(
                   indexed_colors,
                   indexed_colors + lenof(indexed_colors),
                   [&color](Color const &a, Color const &b) {
                       return (a - color).sqr_magnitude() <=
                              (b - color).sqr_magnitude();
                   }
               ) -
               indexed_colors;
    }
};

#endif
