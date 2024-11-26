// man: https://freetype.org/freetype2/docs/tutorial/step1.html

#include "font.hpp"

#include <stdexcept>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include "conviniences.hpp"

// TODO? maybe make library static/global
Font Font::load(char const *const path, float const font_size) {
    FT_Library ft = nullptr;
    let handle_err = [](FT_Error err, char const *const msg = "error") {
        if (err != FT_Err_Ok) throw std::runtime_error(msg);
    };

    handle_err(FT_Init_FreeType(&ft), "Freetype init error.");

    (void)path, (void)font_size;
    // FT_Face face;
    // handle_err(FT_New_Face(ft, path, 0, &face), "Face loading error.");
    //
    // handle_err(
    //     FT_Set_Char_Size(face, 0, font_size * 64, 96, 96),
    //     "Size setting error."
    // );
    // handle_err(FT_Load_Glyph(face, 0, FT_LOAD_DEFAULT), "Glyph loading
    // error."); FT_Glyph_Get_CBox(face->glyph, FT_GLYPH_BBOX);

    // let size = Size(face->bbox.xMin, face->bbox.yMin) / 26.6 / 64;
    // std::cout << size.w << ", " << size.h << "\n";

    return Font(ft);
}
Font::~Font() { FT_Done_FreeType(ft); }
