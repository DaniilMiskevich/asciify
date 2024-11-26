#ifndef IMAGE_FILTER_HPP
#define IMAGE_FILTER_HPP

template <typename T>
class ImageFilter {
   public:
    friend T &operator*=(T &image, ImageFilter const &other) {
        other(image);
        return image;
    }

    virtual void operator()(T &dst) const = 0;
};

#endif
