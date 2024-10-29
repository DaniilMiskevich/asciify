#ifndef FONT_HPP
#define FONT_HPP

class Font {
   public:
    static Font load(char const *const path, double const font_size = 12);

    ~Font();

   private:
    Font(void const *const ft) : ft(ft) {}

    void const *const ft;
};

#endif
