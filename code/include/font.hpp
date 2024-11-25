#ifndef FONT_HPP
#define FONT_HPP

class Font {
    typedef struct FT_LibraryRec_ *FT_Library;

   public:
    static Font load(char const *const path, float const font_size = 12);

    // TODO? copy constructor

    ~Font();

   private:
    Font(FT_Library ft) : ft(ft) {}

    FT_Library const ft;
};

#endif
