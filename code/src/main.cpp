/*
refs:
    - inspiration & everything:
        - https://www.youtube.com/watch?v=gg40RWiaHRY
    - gradients:
        - https://paulbourke.net/dataformats/asciiart/
*/
// TODO add color support for non-RGB terminals

#include <iostream>

#include "ascii/ascii_art.hpp"
#include "ascii/color_ascii_effect.hpp"
#include "ascii/fill_ascii_effect.hpp"
#include "conviniences.hpp"
#include "image/image_loader.hpp"

#define CHAR_SCALE (1.25)

int main() {
    let image_loader = ImageLoader();

    letmut image = image_loader.load("test.webp");
    std::cout << "Image loaded (" << image->get_size().w << "x"
              << image->get_size().h << ")" << "\n";

    let char_size = Size(10, 22) / CHAR_SCALE;
    letmut ascii_art = AsciiArt(*image, char_size);

    let fill = FillAsciiEffect();
    let color = ColorAsciiEffect(true);

    fill(ascii_art);
    color(ascii_art);

    std::cout << ascii_art;

    delete image;

    return 0;
}
