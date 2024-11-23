#ifndef ASCII_ART_WRITER_HPP
#define ASCII_ART_WRITER_HPP

#include <fstream>

#include "ascii/ascii_art.hpp"

class AsciiArtWriter {
   public:
    enum ColorMode { COLOR_MODE_NONE, COLOR_MODE_INDEXED, COLOR_MODE_TRUE };

    AsciiArtWriter(AsciiArt const &art) : art(art) {}

    void write_to(
        std::ostream &stream,
        ColorMode const color_mode = COLOR_MODE_INDEXED
    ) const {
        let size = art.get_size();

        for (unsigned short j = 0; j < size.h; j++) {
            for (unsigned short i = 0; i < size.w; i++) {
                let el = art[Pos(i, j)];

                switch (color_mode) {
                case COLOR_MODE_NONE: stream << el.c; break;

                case COLOR_MODE_INDEXED: {
                    stream << "\033[3" << el.get_indexed_color_index() << ";2m"

                           << el.c

                           << "\033[0m";
                } break;

                case COLOR_MODE_TRUE: {
                    let col = el.get_true_color();

                    stream << "\033[38;2;"

                           << std::to_string(int(col.r * 0xFF)) + ";"
                           << std::to_string(int(col.g * 0xFF)) + ";"
                           << std::to_string(int(col.b * 0xFF)) + "m"

                           << el.c

                           << "\033[0m";
                } break;
                }
            }
            stream << "\n";
        }
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
    AsciiArt const &art;
};

#endif
