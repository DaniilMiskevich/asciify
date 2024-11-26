#ifndef FILTER_HPP
#define FILTER_HPP

#include <type_traits>
template <typename T>
class Image;

template <typename T, typename ImageT = Image<T>>
struct Filter {
    static_assert(std::is_base_of<Image<T>, ImageT>::value);

    virtual void operator()(ImageT &dst) const = 0;
};

#endif
