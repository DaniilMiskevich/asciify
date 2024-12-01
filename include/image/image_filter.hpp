#ifndef IMAGE_FILTER_HPP
#define IMAGE_FILTER_HPP

template <typename T>
class ImageFilter {
   public:
    virtual ~ImageFilter() {}

    friend T const &operator*=(T &image, ImageFilter const &other) {
        other(image);
        return image;
    }

   protected:
    virtual void operator()(T &dst) const = 0;
};

#endif
