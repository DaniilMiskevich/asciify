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
        stream << "mode: " << color_mode << std::endl;
        let size = art.size();

        letmut pos = Pos(0, 0);
        for (pos.y = 0; pos.y < size.h; pos.y++) {
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
            stream << std::endl;
        }
    }

    void write_to_file(
        char const *const filename,
        ColorMode const color_mode = COLOR_MODE_NONE
    ) const {
        letmut out = std::ofstream(filename, std::ios::binary);
        out.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        write_to(out, color_mode);
    }

   private:
    AsciiArt const &art;
};

#endif
