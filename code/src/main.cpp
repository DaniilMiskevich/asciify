/*
refs:
    - inspiration & everything:
        - https://www.youtube.com/watch?v=gg40RWiaHRY
    - gradients:
        - https://paulbourke.net/dataformats/asciiart/
*/

#include <fstream>
#include <iostream>

#include "ascii/ascii_art.hpp"
#include "ascii/ascii_art_writer.hpp"
#include "ascii/color_ascii_effect.hpp"
#include "ascii/edge_ascii_effect.hpp"
#include "ascii/fill_ascii_effect.hpp"
#include "conviniences.hpp"
#include "image/image_loader.hpp"

#define CHAR_SCALE (1.25)

void run() {
    let image_loader = ImageLoader();

    // let font = Font::load("font.ttf");

    let char_size = Size(10, 22) / CHAR_SCALE;

    let fill = FillAsciiEffect(" .,-:+*csS$@");
    let edges = EdgeAsciiEffect(0.5);
    let color = ColorAsciiEffect();

    letmut image = image_loader.decode("test.jpeg");
    // letmut image = image_loader.load("test.webp");
    // letmut image = image_loader.load("CMakeLists.txt");
    std::cout << "Image loaded (" << image->get_size().w << "x"
              << image->get_size().h << ")" << "\n";

    letmut ascii_art = AsciiArt(*image, char_size);

    fill(ascii_art);
    edges(ascii_art);
    color(ascii_art);

    delete image;

    let writer = AsciiArtWriter(ascii_art);
    writer.write_to(std::cout, AsciiArtWriter::COLOR_MODE_TRUE);
    writer.write_to_file("out.txt");
}

int main() {
    try {
        run();
        return 0;
    } catch (ImageLoader::UnsupportedFormatException &e) {
        std::cout << e.what() << "\n";
        return 1;
    }
}
