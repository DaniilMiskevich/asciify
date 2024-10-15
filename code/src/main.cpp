#include <iomanip>
#include <iostream>

#include "conviniences.hpp"
#include "effect/effect.hpp"
#include "image/webp_image.hpp"

int main() {
    let image = WebpImage("test.webp");
    letmut ascii_art = AsciiArt(image.get_size() / 8);

    std::cout << image.get_size().w << "x" << image.get_size().h << "\n";

    let effect = LuminanceAsciiEffect();
    effect(image, ascii_art);

    std::cout << ascii_art;

    return 0;
}
