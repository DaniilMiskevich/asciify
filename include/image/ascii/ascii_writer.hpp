#ifndef ASCII_ART_WRITER_HPP
#define ASCII_ART_WRITER_HPP

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
                    stream << "\033[3" << el.indexed_color_index() << ";2m"

                           << el.c

                           << "\033[0m";
                } break;

                case COLOR_MODE_TRUE: {
                    let col = el.true_color();

                    stream << "\033[38;2;"

                           << int(col.r8()) << ";" << int(col.g8()) << ";"
                           << int(col.b8()) << "m"

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
    Size const frame_size;
    Ascii const &art;
};

#endif
