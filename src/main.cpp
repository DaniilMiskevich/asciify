/*
refs:
    - inspiration & everything:
        - https://www.youtube.com/watch?v=gg40RWiaHRY
    - gradients:
        - https://paulbourke.net/dataformats/asciiart/
    - dog:
        - https://users.cs.northwestern.edu/~sco590/winnemoeller-cag2012.pdf
*/

#include <iostream>

#include "conviniences.hpp"
#include "font.hpp"
#include "image/ascii/ascii_writer.hpp"
#include "image/ascii/filter/color_ascii_filter.hpp"
#include "image/ascii/filter/edge_ascii_filter.hpp"
#include "image/ascii/filter/fill_ascii_filter.hpp"
#include "image/bitmap/codec/bitmap_codec.hpp"

#define FRAME_SIZE_CHARS (Size(100, 50))

void run() {
    let fill = FillAsciiFilter(" .:+*csS&$@");
    let color = ColorAsciiFilter();
    let edges = EdgeAsciiFilter(4.5, "|\\`~;/");

    letmut char_size = Size(10, 20);
    try {
        char_size = Font::load("font.ttf").size();
    } catch (Font::InternalLoadingException const &e) {
        std::cerr << "Font loading failed. Using fallback size instead."
                  << std::endl;
    }
    std::cout << "Using following character size: " << char_size.w << "x"
              << char_size.h << std::endl;

    let codec = BitmapCodec();
    let bitmap = codec.decode("test.webp");
    // let bitmap = codec.decode("test.jpeg");
    // let bitmap = codec.decode("test_cmyk.jpeg");
    // let bitmap = codec.decode("CMakeLists.txt");
    let size = bitmap->size();
    std::cout << "bitmap size: " << size.w << "x" << size.h << std::endl;

    letmut ascii = Ascii(*bitmap, FRAME_SIZE_CHARS - Size(2, 2), char_size);

    ascii *= fill;
    ascii *= edges;
    ascii *= color;

    delete bitmap;

    let writer = AsciiWriter(ascii, FRAME_SIZE_CHARS);
    writer.write_to(std::cout, AsciiWriter::COLOR_MODE_INDEXED);
    writer.write_to_file("out.txt");
}

int main() {
    try {
        run();
        return 0;
    } catch (std::exception const &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
