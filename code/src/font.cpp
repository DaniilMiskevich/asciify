// man: https://freetype.org/freetype2/docs/tutorial/step1.html

#include "font.hpp"

#include <stdexcept>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

// TODO? maybe make library static/global
Font::Font() {
    FT_Library ft = nullptr;
    FT_Error err;

    err = FT_Init_FreeType(&ft);
    if (err != FT_Err_Ok)
        throw std::runtime_error("FreeType initialization error.");

    this->ft = ft;
}
Font::~Font() { FT_Done_FreeType(reinterpret_cast<FT_Library>(this->ft)); }
