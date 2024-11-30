#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <algorithm>

#include "conviniences.hpp"
#include "dims.hpp"

template <typename T>
class Image {
   public:
    class Iterator;
    class Region;

    Size size(void) const { return _size; }

    T const &operator[](Pos pos) const {
        if (int16_t(pos.x) < 0)
            pos.x = 0;
        else if (pos.x >= _size.w)
            pos.x = _size.w - 1;

        if (int16_t(pos.y) < 0)
            pos.y = 0;
        else if (pos.y >= _size.h)
            pos.y = _size.h - 1;

        return data[pos.x + pos.y * _size.w];
    }
    T &operator[](Pos pos) {
        if (int16_t(pos.x) < 0)
            pos.x = 0;
        else if (pos.x >= _size.w)
            pos.x = _size.w - 1;

        if (int16_t(pos.y) < 0)
            pos.y = 0;
        else if (pos.y >= _size.h)
            pos.y = _size.h - 1;

        return data[pos.x + pos.y * _size.w];
    }

    Iterator begin() const { return Iterator(Pos(0, 0), *this); }
    Iterator end() const { return Iterator(Pos(0, _size.h), *this); }

   protected:
    explicit Image(Size const size) : _size(size), data(new T[size.area()]()) {}
    explicit Image(Image const &other)
    : _size(other._size), data(new T[_size.area()]()) {
        std::copy(other.data, other.data + _size.area(), data);
    }

    ~Image() { delete[] data; };

   private:
    Size const _size;
    T *const data;
};

template <typename T>
class Image<T>::Iterator {
   public:
    Iterator(Pos const pos, Image const &image)
    : _pos(pos),
      image_size(image._size),
      region_size(image._size),
      data(image.data) {}

    Iterator(
        Pos const pos,
        Image const &image,
        Pos const region_offset,
        Size const region_size
    )
    : _pos(pos),
      image_size(image._size),
      region_size(region_size),
      data(image.data + region_offset.x + region_offset.y * image._size.w) {}

    Pos pos() const { return _pos; }

    Iterator &operator++() {
        _pos.x++;
        if (_pos.x < region_size.w) return *this;

        _pos.x = 0;
        _pos.y++;
        return *this;
    }

    Iterator operator++(int) {
        let it = *this;
        ++(*this);
        return it;
    }

    T const &operator*() const { return data[_pos.x + _pos.y * image_size.w]; }
    T &operator*() { return data[_pos.x + _pos.y * image_size.w]; }

    T const *operator->() const {
        return data + _pos.x + _pos.y * image_size.w;
    }
    T *operator->() { return data + _pos.x + _pos.y * image_size.w; }

    bool operator==(Iterator const &other) const { return _pos == other._pos; }
    bool operator!=(Iterator const &other) const { return _pos != other._pos; }

   private:
    Pos _pos;
    Size const image_size, region_size;
    T *const data;
};

template <typename T>
class Image<T>::Region {
   public:
    Region(Image const &base, Pos const offset, Size const size)
    : offset(offset),
      _size(Size(
          std::min<uint16_t>(size.w, base.size().w - offset.x),
          std::min<uint16_t>(size.h, base.size().h - offset.y)
      )),
      base(base) {
        let image_size = base.size();
        assert(offset.x <= image_size.w && offset.y <= image_size.h);
    }

    Size size() const { return _size; }

    T const &operator[](Pos const pos) const { return base[offset + pos]; }
    T &operator[](Pos const pos) { return base[offset + pos]; }

    Iterator begin() const {
        return Image::Iterator(Pos(0, 0), base, offset, _size);
    }
    Iterator end() const {
        return Image::Iterator(Pos(0, _size.h), base, offset, _size);
    }

   private:
    Pos const offset;
    Size const _size;
    Image const &base;
};

#endif
