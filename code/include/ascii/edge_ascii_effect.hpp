#ifndef EDGE_ASCII_EFFECT_HPP
#define EDGE_ASCII_EFFECT_HPP

#include <cmath>
#include <iostream>

#include "ascii/ascii_effect.hpp"

class SobelFilter {
   public:
    struct El {
        El() = delete;

        double const x, y;
    };

    SobelFilter(Image const &src) : src(src) {}

    Size get_size(void) const { return src.get_size(); }

    El sum_region(Pos const pos, Size const size) const {
        letmut s_x = 0.0, s_y = 0.0;
        for (letmut dx = 0; dx < size.w; dx++)
            for (letmut dy = 0; dy < size.h; dy++) {
                let pix =
                    (*this)[Pos(pos.x * size.w + dx, pos.y * size.h + dy)];
                s_x += pix.x, s_y += pix.y;
            }

        return El{s_x, s_y};
    }

    El operator[](Pos const pos) const {
        static double const gx[3][3] = {
            {+1, +0, -1},
            {+2, +0, -2},
            {+1, +0, -1},
        };
        static double const gy[3][3] = {
            {+1, +2, +1},
            {+0, +0, +0},
            {-1, -2, -1},
        };
        Color const src_pxs[3][3] = {
            {src[pos - 1], src[pos + Pos(0, -1)], src[pos + Pos(1, -1)]},
            {src[pos + Pos(-1, 0)], src[pos], src[pos + Pos(1, 0)]},
            {src[pos + Pos(-1, 1)], src[pos + Pos(0, 1)], src[pos + 1]},
        };

        let x = Image::filter(src_pxs, gx), y = Image::filter(src_pxs, gy);
        return El{x, y};
    }

   private:
    Image const &src;
};

class EdgeAsciiEffect : public AsciiEffect {
   public:
    void operator()(AsciiArt &dst) const override {
        let edges = SobelFilter(dst.get_image());
        let size = dst.get_size();
        let char_size = dst.get_image().get_size() / size;

        for (letmut i = 0; i < size.w; i++) {
            for (letmut j = 0; j < size.h; j++) {
                let pos = Pos(i, j);

                let edge0 = edges.sum_region(pos, edges.get_size() / char_size);
                let magnitude = sqrt(edge0.x * edge0.x + edge0.y * edge0.y);
                if (magnitude == 0) continue;
                let edge = SobelFilter::El{
                    edge0.x * sqrt(2) / magnitude,
                    edge0.y * sqrt(2) / magnitude
                };

                std::cout << round(edge.x * 100) / 100 << "|"
                          << round(edge.y * 100) / 100 << "\n";

                let c = [](int x, int y) {
                    static char const cs[3][3] = {
                        {'/', '-', '\\'},
                        {'|', '\0', '|'},
                        {'\\', '-', '/'},
                    };
                    return cs[y][x];
                }(round(edge.x) + 1, round(edge.y) + 1);
                if (c) { dst[pos] = AsciiEl(c); }
            }
        }
    }
};

#endif
