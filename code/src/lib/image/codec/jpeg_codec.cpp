#include "image/codec/jpeg_codec.hpp"

#include <algorithm>
#include <cassert>
#include <cstdio>

#include <jpeglib.h>

#include "conviniences.hpp"

static void my_jpeg_error_exit(j_common_ptr const common) {
    throw JpegCodec::InternalDecodingException(common->err->msg_parm.s);
}
static struct jpeg_error_mgr *jpeg_throw_error(struct jpeg_error_mgr *err) {
    err = jpeg_std_error(err);
    err->error_exit = my_jpeg_error_exit;
    return err;
}

template <typename T>
static T const *
jpeg_read_pixels(jpeg_decompress_struct &info, Size const size) {
    let buf = new T[size.area()];
    while (info.output_scanline < size.h) {
        letmut buf_ptr = buf + info.output_scanline * size.w;
        jpeg_read_scanlines(&info, reinterpret_cast<uint8_t **>(&buf_ptr), 1);
    }

    return buf;
}

Image<Color> *
JpegCodec::decode(uint8_t const *const src_data, size_t const src_size) {
    struct RGBPixel {
        uint8_t r, g, b;
    };
    struct CMYKPixel {
        uint8_t inv_c, inv_m, inv_y, k;
    };
    struct GrayPixel {
        uint8_t g;
    };

    jpeg_decompress_struct info;
    jpeg_error_mgr err;
    try {
        info.err = jpeg_throw_error(&err);
        jpeg_create_decompress(&info);

        jpeg_mem_src(&info, src_data, src_size);

        if (!jpeg_read_header(&info, 1)) throw InvalidHeaderDecodingException();
        if (!jpeg_start_decompress(&info)) throw std::bad_alloc();

        let size = Size(info.image_width, info.image_height);

        let image = new Image<Color>(size);
        switch (info.output_components) {
        case sizeof(RGBPixel): {
            let buf = jpeg_read_pixels<RGBPixel>(info, size);
            std::transform(
                buf,
                buf + size.area(),
                image->begin(),
                [](RGBPixel const px) { return Color::rgb24(px.r, px.g, px.b); }
            );
            delete[] buf;
        }; break;

        case sizeof(CMYKPixel): {
            let buf = jpeg_read_pixels<CMYKPixel>(info, size);
            std::transform(
                buf,
                buf + size.area(),
                image->begin(),
                [](CMYKPixel const px) {
                    return Color(px.inv_c, px.inv_m, px.inv_y) / 0xFF * px.k /
                           0xFF;
                }
            );
            delete[] buf;
        }; break;

        case sizeof(GrayPixel): {
            let buf = jpeg_read_pixels<GrayPixel>(info, size);
            std::transform(
                buf,
                buf + size.area(),
                image->begin(),
                [](GrayPixel const px) { return Color(0xFFFFFF) * px.g / 0xFF; }
            );
            delete[] buf;
        }; break;

        default: throw InternalDecodingException("Unsupported color format.");
        }

        jpeg_finish_decompress(&info);
        jpeg_destroy_decompress(&info);

        return image;
    } catch (std::exception const &) {
        jpeg_destroy_decompress(&info);
        throw;
    }
}
