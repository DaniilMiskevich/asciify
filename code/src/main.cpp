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
#include "image/codec/image_codec.hpp"

#define CHAR_SIZE        (Size(10, 21))
#define FRAME_SIZE_CHARS (Size(100, 40))

void run() {
    let fill = FillAsciiFilter(" .:+*csS&$@");
    let color = ColorAsciiFilter();
    let edges = EdgeAsciiFilter(4.5, "|\\`~;/");

    let codec = ImageCodec();
    // let image = codec.decode("test.webp");
    // let image = codec.decode("test.jpeg");
    let image = codec.decode("test_cmyk.jpeg");
    // let image = codec.decode("CMakeLists.txt");
    let image_size = image->size();
    std::cout << "image size: " << image_size.w << "x" << image_size.h
              << std::endl;

    letmut ascii_art =
        AsciiArt(*image, FRAME_SIZE_CHARS - Size(2, 2), CHAR_SIZE);

    ascii_art *= fill;
    ascii_art *= edges;
    ascii_art *= color;

    delete image;

    let writer = AsciiArtWriter(ascii_art, FRAME_SIZE_CHARS);
    writer.write_to(std::cout, AsciiArtWriter::COLOR_MODE_TRUE);
    writer.write_to_file("out.txt");
}

int main() {
    try {
        run();
        return 0;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
