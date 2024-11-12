//
// #include "image/image.hpp"
// class FilteredImage : public Image {
//    public:
//     FilteredImage(Color const *const data, Size const size)
//     : data(data), size(size) {}
//
//     Size get_size(void) const { return size; }
//     Color operator[](Pos const pos) const {
//         return data[pos.x + size.w * pos.y];
//     }
//
//    private:
//     Color const *const data;
//     Size const size;
// };
//
// template <size_t W, size_t H>
// class Filter {
//    public:
//     Image &operator*(Image const &other) {
//         let size = other.get_size();
//         letmut data = new Color[size.get_area()];
//         // letmut s = T(0);
//         for (letmut i = uint16_t(0); i < W; i++)
//             for (letmut j = uint16_t(0); j < H; j++)
//                 s += src[i][j] * kernel[i][j];
//
//         return s;
//     }
//
//    private:
//
// };
//
// class SobelFilter : public Filter {
//    public:
//     SobelFilter(Image const &src) : src(src) {}
//
//     Size get_size(void) const { return src.get_size(); }
//     // TODO? maybe worth to compute once in the constructor
//     Color operator[](Pos const pos) const {
//         static double const gx[3][3] = {
//             {+1, +0, -1},
//             {+2, +0, -2},
//             {+1, +0, -1},
//         };
//         static double const gy[3][3] = {
//             {+1, +2, +1},
//             {+0, +0, +0},
//             {-1, -2, -1},
//         };
//         double const src_pxs[3][3] = {
//             {
//                 src[pos - 1].get_magnitude(),
//                 src[pos + Pos(0, -1)].get_magnitude(),
//                 src[pos + Pos(1, -1)].get_magnitude(),
//             },
//             {
//                 src[pos + Pos(-1, 0)].get_magnitude(),
//                 src[pos].get_magnitude(),
//                 src[pos + Pos(1, 0)].get_magnitude(),
//             },
//             {
//                 src[pos + Pos(-1, 1)].get_magnitude(),
//                 src[pos + Pos(0, 1)].get_magnitude(),
//                 src[pos + 1].get_magnitude(),
//             },
//         };
//
//         let x = Image::filter(src_pxs, gx), y = Image::filter(src_pxs, gy);
//
//         return Color(x, y, 0);
//     }
//
//    private:
//     Image const &src;
// };
