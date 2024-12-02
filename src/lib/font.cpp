// man: https://freetype.org/freetype2/docs/tutorial/step1.html

#include "font.hpp"

#include <cmath>
#include <fstream>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include "conviniences.hpp"
#include "dims.hpp"

static struct FTWrap {
    FTWrap()
    : lib([]() {
          FT_Library ft = nullptr;
          handle_err(FT_Init_FreeType(&ft), "Freetype init error.");
          return ft;
      }()) {}
    ~FTWrap() { FT_Done_FreeType(lib); }

    FT_Library const lib;

    static void
    handle_err(FT_Error const err, char const *const msg = "Unknown error.") {
        if (err != FT_Err_Ok) throw Font::InternalLoadingException(msg);
    };

   private:
} ft;

Font Font::load(char const *const path, float const font_size) {
    assert(font_size >= 8);

    letmut src = std::ifstream(path, std::ios::binary);
    src.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    src.seekg(0, std::ios::end);
    let src_size = src.tellg();
    src.seekg(0, std::ios::beg);

    let src_data = new uint8_t[src_size]();
    src.read(reinterpret_cast<char *>(src_data), src_size);

    FT_Face face;
    FTWrap::handle_err(
        FT_New_Memory_Face(ft.lib, src_data, src_size, 0, &face),
        "Face loading error."
    );

    delete[] src_data;

    let font_size_px = font_size * float(96.0 / 72.0);

    FTWrap::handle_err(
        FT_Set_Pixel_Sizes(face, 0, font_size_px),
        "Face loading error."
    );

    let size = Size(face->max_advance_width, face->size->metrics.height) *
               (font_size_px / face->units_per_EM);

    FT_Done_Face(face);

    return Font(size);
}
