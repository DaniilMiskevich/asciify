#include "ascii/color_ascii_effect.hpp"

template <>
void ColorAsciiEffect<false>::operator()(AsciiArt &dst) const {
    static Color const colors[] = {
        Color(0x000000),
        Color(0x800000),
        Color(0x008000),
        Color(0x808000),
        Color(0x000080),
        Color(0x800080),
        Color(0x008080),
        Color(0xc0c0c0),
    };

    let &image = dst.get_image();
    let size = dst.get_size();

    for (letmut i = 0; i < size.w; i++) {
        for (letmut j = 0; j < size.h; j++) {
            let pos = Pos(i, j);

            let avg_col = image.get_avg_in_region(pos, image.get_size() / size);

            let col_i = [avg_col]() {
                size_t closest_i;
                for (letmut i = closest_i = size_t(1); i < lenof(colors); i++) {
                    if (Color::distance(colors[i], avg_col) <=
                        Color::distance(colors[closest_i], avg_col))
                        closest_i = i;
                }
                return closest_i;
            }();

            dst[pos] = AsciiEl(
                "\033[3" + std::to_string(col_i) + ";2m" +

                dst[pos].data +

                "\033[0m"
            );
        }
    }
}

template <>
void ColorAsciiEffect<true>::operator()(AsciiArt &dst) const {
    let &image = dst.get_image();
    let size = dst.get_size();

    for (letmut i = 0; i < size.w; i++) {
        for (letmut j = 0; j < size.h; j++) {
            let pos = Pos(i, j);

            let avg_col = image.get_avg_in_region(pos, image.get_size() / size);

            dst[pos] = AsciiEl(
                "\033[38;2;" +

                std::to_string(int(avg_col.r)) + ";" +
                std::to_string(int(avg_col.g)) + ";" +
                std::to_string(int(avg_col.b)) + "m" +

                dst[pos].data +

                "\033[0m"
            );
        }
    }
}
