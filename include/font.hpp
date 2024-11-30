#ifndef FONT_HPP
#define FONT_HPP

#include <exception>

#include "dims.hpp"

class Font {
    typedef struct FT_LibraryRec_ *FT_Library;

   public:
    static Font load(char const *const path, float const font_size = 12);

    Size size() { return _size; }

   private:
    explicit Font(Size const size) : _size(size) {}

    Size const _size;

   public:
    struct InternalLoadingException;
};

struct Font::InternalLoadingException : public std::exception {
    InternalLoadingException(char const *const msg) : msg(msg) {}

    char const *const msg;

    char const *what() const throw() override { return msg; }
};

#endif
