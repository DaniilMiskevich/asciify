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

#include "ascii/ascii_art.hpp"
#include "ascii/ascii_art_writer.hpp"
#include "ascii/filter/color_ascii_filter.hpp"
#include "ascii/filter/edge_ascii_filter.hpp"
#include "ascii/filter/fill_ascii_filter.hpp"
#include "conviniences.hpp"
#include "font.hpp"
#include "image/codec/image_codec.hpp"

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

    let codec = ImageCodec();
    let image = codec.decode("test.webp");
    // let image = codec.decode("test.jpeg");
    // let image = codec.decode("test_cmyk.jpeg");
    // let image = codec.decode("CMakeLists.txt");
    let image_size = image->size();
    std::cout << "image size: " << image_size.w << "x" << image_size.h
              << std::endl;

    letmut ascii_art =
        AsciiArt(*image, FRAME_SIZE_CHARS - Size(2, 2), char_size);

    ascii_art *= fill;
    ascii_art *= edges;
    ascii_art *= color;

    delete image;

    let writer = AsciiArtWriter(ascii_art, FRAME_SIZE_CHARS);
    writer.write_to(std::cout, AsciiArtWriter::COLOR_MODE_INDEXED);
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
