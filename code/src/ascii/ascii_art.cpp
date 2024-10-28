#include "ascii/ascii_art.hpp"

std::ostream &operator<<(std::ostream &stream, AsciiArt const &ascii_art) {
    for (unsigned short j = 0; j < ascii_art.size.h; j++) {
        for (unsigned short i = 0; i < ascii_art.size.w; i++)
            stream << ascii_art[Pos(i, j)];
        stream << "\n";
    }

    return stream;
}
