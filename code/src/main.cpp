/*
refs:
    - inspiration & everything:
        - https://www.youtube.com/watch?v=gg40RWiaHRY
    - gradients:
        - https://paulbourke.net/dataformats/asciiart/
*/

#include <iostream>

#include "conviniences.hpp"
#include "effect/color_ascii_effect.hpp"
#include "effect/fill_ascii_effect.hpp"
#include "image/image_loader.hpp"

int main() {
    let image_loader = ImageLoader();

    letmut image = image_loader.load("test.jpeg");
    std::cout << image->get_size().w << "x" << image->get_size().h << "\n";

    letmut ascii_art = AsciiArt(image->get_size(), Size(10, 22) / 1.25);

    let fill_effect = FillAsciiEffect();
    let color_effect = ColorAsciiEffect();
    // TODO not looking good for me
    fill_effect(*image, ascii_art);
    color_effect(*image, ascii_art);

    std::cout << ascii_art;

    delete image;

    return 0;
}
