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

Image<Color> *
JpegCodec::decode(uint8_t const *const src_data, size_t const src_size) {
    struct Pixel {
        uint8_t r, g, b;
    };

    jpeg_decompress_struct info;
    jpeg_error_mgr err;
    try {
        info.err = jpeg_throw_error(&err);
        jpeg_create_decompress(&info);

        jpeg_mem_src(&info, src_data, src_size);

        if (!jpeg_read_header(&info, 1)) throw InvalidHeaderDecodingException();
        if (!jpeg_start_decompress(&info)) throw std::bad_alloc();

        // TODO! support CMYK and grayscale images
        if (info.output_components != sizeof(Pixel))
            throw InternalDecodingException("Unsupported colour format.");

        let size = Size(info.image_width, info.image_height);

        let buf = new Pixel[size.area()];
        while (info.output_scanline < size.h) {
            Pixel *buf_ptr = buf + info.output_scanline * size.w;

            jpeg_read_scanlines(
                &info,
                reinterpret_cast<uint8_t **>(&buf_ptr),
                1
            );
        }

        let image = new Image<Color>(size);
        std::transform(
            buf,
            buf + size.area(),
            image->begin(),
            [](Pixel const px) { return Color::rgb24(px.r, px.g, px.b); }
        );

        delete[] buf;

        jpeg_finish_decompress(&info);
        jpeg_destroy_decompress(&info);

        return image;
    } catch (std::exception const &e) {
        jpeg_destroy_decompress(&info);
        throw;
    }
}
