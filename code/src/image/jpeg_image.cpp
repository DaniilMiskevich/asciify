#include "image/jpeg_image.hpp"

#include <cassert>
#include <cstdio>

#include <jpeglib.h>

#include "conviniences.hpp"

static void my_jpeg_error_exit(j_common_ptr const common) {
    throw JpegImage::InternalLoadingException(common->err->msg_parm.s);
}
static struct jpeg_error_mgr *jpeg_throw_error(struct jpeg_error_mgr *err) {
    err = jpeg_std_error(err);
    err->error_exit = my_jpeg_error_exit;
    return err;
}

JpegImage
JpegImage::decode(uint8_t const *const src_data, size_t const src_size) {
    jpeg_decompress_struct info;
    jpeg_error_mgr err;
    try {
        info.err = jpeg_throw_error(&err);
        jpeg_create_decompress(&info);

        jpeg_mem_src(&info, src_data, src_size);

        if (!jpeg_read_header(&info, 1)) throw InvalidHeaderLoadingException();

        // TODO
        assert(jpeg_start_decompress(&info));
        // TODO! support CMYK and grayscale images
        if (info.output_components != sizeof(Pixel))
            throw InternalLoadingException("Unsupported colour format.");

        let width = info.image_width;
        let height = info.image_height;

        let buf = new Pixel[width * height];

        while (info.output_scanline < height) {
            Pixel *buf_ptr = buf + info.output_scanline * width;

            jpeg_read_scanlines(
                &info,
                reinterpret_cast<uint8_t **>(&buf_ptr),
                1
            );
        }

        jpeg_finish_decompress(&info);
        jpeg_destroy_decompress(&info);

        return JpegImage(buf, Size(width, height));
    } catch (std::exception &e) {
        jpeg_destroy_decompress(&info);
        throw;
    }
}

JpegImage::~JpegImage() { delete[] data; }
