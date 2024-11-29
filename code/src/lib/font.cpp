// man: https://freetype.org/freetype2/docs/tutorial/step1.html

#include "font.hpp"

#include <cmath>

#include <freetype2/ft2build.h>

#include "dims.hpp"
#include FT_FREETYPE_H

#include "conviniences.hpp"

static struct FTWrap {
    FTWrap() : lib(load_lib()) {}

    FT_Library const lib;

    static void
    handle_err(FT_Error const err, char const *const msg = "Unknown error.") {
        if (err != FT_Err_Ok) throw Font::InternalLoadingException(msg);
    };

   private:
    static FT_Library load_lib() {
        FT_Library ft = nullptr;
        handle_err(FT_Init_FreeType(&ft), "Freetype init error.");
        return ft;
    }

} ft;

Font Font::load(char const *const path, float const font_size_px) {
    FT_Face face;
    FTWrap::handle_err(
        FT_New_Face(ft.lib, path, 0, &face),
        "Face loading error."
    );

    let size = Size(
                   face->bbox.yMax - face->bbox.yMin,
                   face->bbox.xMax - face->bbox.xMin
               ) *
               font_size_px / face->units_per_EM;

    return Font(size);
}
