#include <iostream>

#include "conviniences.hpp"
#include "image/webp_image.hpp"

int main() {
    let image = WebpImage("test.webp");

    std::cout << image.get_size().w << "\n";

    return 0;
}
