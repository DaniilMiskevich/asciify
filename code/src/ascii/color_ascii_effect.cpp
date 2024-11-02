#include "ascii/color_ascii_effect.hpp"

template <>
void ColorAsciiEffect<false>::operator()(AsciiArt &dst) const {
    static Color const cols[] = {
        Color::hex(0x000000),
        Color::hex(0x800000),
        Color::hex(0x008000),
        Color::hex(0x808000),
        Color::hex(0x000080),
        Color::hex(0x800080),
        Color::hex(0x008080),
        Color::hex(0xC0C0C0),
    };

    let &image = dst.get_image();
    let size = dst.get_size();

    letmut pos = Pos(0, 0);
    for (pos.x = 0; pos.x < size.w; pos.x++) {
        for (pos.y = 0; pos.y < size.h; pos.y++) {
            let avg_col = image.get_avg_in_region(pos, image.get_size() / size);

            let i = [avg_col]() {
                size_t closest_i;
                for (letmut i = closest_i = size_t(1); i < lenof(cols); i++) {
                    if ((cols[i] - avg_col).get_magnitude() <=
                        (cols[closest_i] - avg_col).get_magnitude())
                        closest_i = i;
                }
                return closest_i;
            }();

            dst[pos] = AsciiEl(
                "\033[3" + std::to_string(i) + ";2m" +

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

    letmut pos = Pos(0, 0);
    for (pos.x = 0; pos.x < size.w; pos.x++) {
        for (pos.y = 0; pos.y < size.h; pos.y++) {
            let avg_col = image.get_avg_in_region(pos, image.get_size() / size);

            dst[pos] = AsciiEl(
                "\033[38;2;" +

                std::to_string(int(avg_col.r * 0xFF)) + ";" +
                std::to_string(int(avg_col.g * 0xFF)) + ";" +
                std::to_string(int(avg_col.b * 0xFF)) + "m" +

                dst[pos].data +

                "\033[0m"
            );
        }
    }
}
