#include "image/jpeg_image.hpp"

#include <cassert>
#include <fstream>
#include <iostream>

#include <jpeglib.h>

#include "conviniences.hpp"

JpegImage::JpegImage(char const *const filename) {
    static_assert(
        sizeof(char) == sizeof(uint8_t),
        "This code is expected to work on 1-byte chars only."
    );

    letmut src = std::ifstream(filename, std::ios::binary);
    src.seekg(0, std::ios::end);
    let src_size = src.tellg();
    src.seekg(0, std::ios::beg);

    let src_data = new uint8_t[src_size];
    if (src_data == nullptr) throw std::runtime_error("Allocatoin failed!");
    src.read(reinterpret_cast<char *>(src_data), src_size);

    read_jpeg_data(src_data, src_size);

    delete[] src_data;
}

void JpegImage::read_jpeg_data(
    uint8_t const *const src_data,
    size_t const src_size
) {
    struct jpeg_decompress_struct info;
    struct jpeg_error_mgr jerr;

    info.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&info);

    jpeg_mem_src(&info, src_data, src_size);

    if (!jpeg_read_header(&info, TRUE))
        throw std::runtime_error("Cannot read jpeg.");

    jpeg_start_decompress(&info);
    if (info.output_components != sizeof(JpegPixel))
        throw std::runtime_error("Unsupported colour format.");

    let width = info.image_width;
    let height = info.image_height;

    let buf = new JpegPixel[width * height];

    while (info.output_scanline < height) {
        JpegPixel *buf_ptr = buf + info.output_scanline * width;

        jpeg_read_scanlines(&info, reinterpret_cast<uint8_t **>(&buf_ptr), 1);
    }

    jpeg_finish_decompress(&info);
    jpeg_destroy_decompress(&info);

    size = Size(width, height);
    this->data = buf;
}

JpegImage::~JpegImage() { delete[] data; }
