#include <cmath>

template <typename T>
struct Vec3 {
    constexpr Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    T const x, y, z;

    constexpr Vec3 operator+(Vec3 const &other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    constexpr Vec3 operator-(Vec3 const &other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    constexpr Vec3 operator*(T const &other) const {
        return Vec3(x * other, y * other, z * other);
    }
    constexpr Vec3 operator/(T const &other) const {
        return Vec3(x / other, y / other, z / other);
    }

    constexpr double magnitude() { return sqrt(x * x + y * y + z * z); }
};
