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
#include "ascii/effect/color_ascii_effect.hpp"
#include "ascii/effect/edge_ascii_effect.hpp"
#include "ascii/effect/fill_ascii_effect.hpp"
#include "conviniences.hpp"
#include "image/image_loader.hpp"

#define FONT_CHAR_SIZE (Size(10, 22))
#define CHAR_SCALE     (1.25)

void run() {
    let fill = FillAsciiEffect(" .:+*csS&$@");
    let color = ColorAsciiEffect();
    let edges = EdgeAsciiEffect(4.5, "|\\`~;/");

    let image_loader = ImageLoader();
    letmut image = image_loader.decode("test.jpeg");
    // letmut image = image_loader.decode("test.webp");
    // letmut image = image_loader.decode("CMakeLists.txt");
    let image_size = image->get_size();
    std::cout << "image size: " << image_size.w << "x" << image_size.h << "\n";

    letmut ascii_art = AsciiArt(*image, FONT_CHAR_SIZE / CHAR_SCALE);

    fill(ascii_art);
    edges(ascii_art);
    color(ascii_art);

    delete image;

    let writer = AsciiArtWriter(ascii_art);
    writer.write_to(std::cout, AsciiArtWriter::COLOR_MODE_INDEXED);
    writer.write_to_file("out.txt");
}

int main() {
    try {
        run();
        return 0;
    } catch (std::exception &e) {
        std::cout << e.what() << "\n";
        return 1;
    }
}
