/*
refs:
    - inspiration & everything:
        - https://www.youtube.com/watch?v=gg40RWiaHRY
    - gradients:
        - https://paulbourke.net/dataformats/asciiart/
        - https://www.youtube.com/watch?v=n4zUgtDk95w
*/
// TODO! if format is invalid aborts on jpeg instead of throwing "invalid
// format"

#include <iostream>

#include "conviniences.hpp"
#include "effect/effect.hpp"
#include "image/image_factory.hpp"

int main() {
    let image_factory = ImageFactory();

    letmut image = image_factory.create("test.jpeg");
    std::cout << image->get_size().w << "x" << image->get_size().h << "\n";

    letmut ascii_art = AsciiArt(image->get_size(), Size(10, 22) / 1.5);

    let effect = LuminanceAsciiEffect();
    // TODO not looking good for me
    effect(*image, ascii_art);

    std::cout << ascii_art;

    delete image;

    return 0;
}
