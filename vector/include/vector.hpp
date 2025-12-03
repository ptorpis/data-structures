/*
 * This project server as an exercise in the fundamentals of C++
 *
 * The purpose is to create a container that works similarly to std::vector
 * while learning along the way.
 */

#include <cstddef>
#include <memory>

namespace ptorpis {
template <typename T, typename Allocator = std::allocator<T>> class vector {
public:
    vector() : data_m(nullptr), capacity_m(0), size_m(0) {}
    vector(std::size_t count, const T& value);

private:
    T* data_m;
    std::size_t capacity_m;
    std::size_t size_m;
};

} // namespace ptorpis
