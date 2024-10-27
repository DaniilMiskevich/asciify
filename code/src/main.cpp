/*
refs:
    - inspiration & everything:
        - https://www.youtube.com/watch?v=gg40RWiaHRY
    - gradients:
        - https://paulbourke.net/dataformats/asciiart/
*/
// TODO! if format is invalid aborts on jpeg instead of throwing "invalid
// format"

#include <iostream>

#include "conviniences.hpp"
#include "effect/fill_ascii_effect.hpp"
#include "image/image_loader.hpp"

int main() {
    let image_loader = ImageLoader();

    letmut image = image_loader.load("test.jpeg");
    std::cout << image->get_size().w << "x" << image->get_size().h << "\n";

    letmut ascii_art = AsciiArt(image->get_size(), Size(10, 22) / 1.25);

    let effect = FillAsciiEffect();
    // TODO not looking good for me
    effect(*image, ascii_art);

    std::cout << ascii_art;

    delete image;

    return 0;
}
