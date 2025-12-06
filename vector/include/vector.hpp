/*
 * @file data-structures/src/vector.hpp
 * @brief Custom implementation of a dynamic array, similar to std::vector<T>
 * @author ptorpis -- Peter Torpis
 *
 * This project server as an exercise in the fundamentals of C++
 *
 * The purpose is to create a container that works similarly to std::vector
 * while learning along the way.
 */

#include <cstddef>
#include <initializer_list>
#include <limits>
#include <memory>
#include <stdexcept>

namespace ptorpis {
/*
 * @brief Dynamic array container
 * @tparam T The type of the elements
 * @tparam Allocator The allocator type used for memory management
 */
template <typename T, typename Allocator = std::allocator<T>> class vector {
private:
    using alloc_traits = std::allocator_traits<Allocator>;

public:
    static std::size_t max_size() { return std::numeric_limits<T>::max(); }

    /*
     * Constructors
     */

    // Default constructor with no parameters, sets T* to nullptr, capacity and size to 0
    vector() noexcept : data_m(nullptr), capacity_m(0), size_m(0) {}

    // Default constructor with allocator, same as default with no params, except it sets
    // the allocator
    vector(const Allocator& allocator) noexcept
        : alloc_m(allocator), data_m(nullptr), capacity_m(0), size_m(0) {}

    /*
     * @brief count constructor: constructs vector object, allocated memory and default
     * constructs all elements.
     * @param std::size_t count: number of elements to allocate memory for and default
     * construct
     * @param const Allocator&: allocator (from template)
     */

    vector(std::size_t count, const Allocator& allocator = Allocator())
        : alloc_m(allocator), data_m(nullptr), capacity_m(0), size_m(0) {
        if (count == 0) {
            return;
        }

        data_m = alloc_traits::allocate(alloc_m, count);
        capacity_m = count;

        try {
            for (std::size_t i{}; i < count; ++i) {
                alloc_traits::construct(alloc_m, data_m + i);
                ++size_m;
            }
        } catch (...) {
            for (std::size_t i{}; i < size_m; ++i) {
                alloc_traits::destroy(alloc_m, data_m + i);
            }

            alloc_traits::deallocate(alloc_m, data_m, capacity_m);

            throw;
        }
    }

    /*
     * @brief Count + Value Constructor: Constructs the vector object, allocates memory
     * and constructs all elements with the value given
     * @param std::size_t count
     * @param const T& value: the value with which the elements should be constructed with
     * @param const Allocator& allocator
     */
    vector(std::size_t count, const T& value, const Allocator& allocator = Allocator())
        : alloc_m(allocator), data_m(nullptr), capacity_m(0), size_m(0) {

        data_m = alloc_traits::allocate(alloc_m, count);
        capacity_m = count;

        try {
            for (std::size_t i{}; i < count; ++i) {
                alloc_traits::construct(alloc_m, data_m + i, value);
                ++size_m;
            }

        } catch (...) {
            for (std::size_t i{}; i < size_m; ++i) {
                alloc_traits::destroy(alloc_m, data_m + i);
            }

            alloc_traits::deallocate(alloc_m, data_m, capacity_m);

            throw;
        }
    }

    vector(std::initializer_list<T> init, const Allocator& allocator = Allocator())
        : alloc_m(allocator), data_m(nullptr), capacity_m(0), size_m(0) {
        std::size_t count = init.size();
        if (count == 0) {
            return;
        }

        data_m = alloc_traits::allocate(alloc_m, count);
        capacity_m = count;

        try {
            auto it = init.begin();
            for (std::size_t i{}; i < count; ++i, ++it) {
                alloc_traits::construct(alloc_m, data_m + i, *it);
                ++size_m;
            }

        } catch (...) {
            for (std::size_t i{}; i < size_m; ++i) {
                alloc_traits::destroy(alloc_m, data_m + i);
            }

            alloc_traits::deallocate(alloc_m, data_m, capacity_m);
            throw;
        }
    }

    vector(const vector& other)
        : alloc_m(alloc_traits::select_on_container_copy_construction(other.alloc_m)),
          data_m(nullptr), capacity_m(0), size_m(0) {
        if (other.size_m == 0) {
            return;
        }

        data_m = alloc_traits::allocate(alloc_m, other.size_m);
        capacity_m = other.size();

        try {
            for (std::size_t i{}; i < other.size_m; ++i) {
                alloc_traits::construct(alloc_m, data_m + i, other.data_m[i]);
                ++size_m;
            }

        } catch (...) {
            for (std::size_t i{}; i < size_m; ++i) {
                alloc_traits::destroy(alloc_m, data_m + i);
            }

            alloc_traits::deallocate(alloc_m, data_m, capacity_m);
            throw;
        }
    }

    vector(vector&& other);

    vector& operator=(const vector& other);
    vector& operator=(vector&& other) noexcept;

    ~vector() = default;

    T& at(std::size_t pos) {
        if (pos >= size_m) {
            throw std::out_of_range("Element accessed is out of bounds");
        }

        return data_m[pos];
    }

    const T& at(std::size_t pos) const {
        if (pos >= size_m) {
            throw std::out_of_range("Element accessed is out of bounds");
        }
        return data_m[pos];
    }

    T& operator[](std::size_t pos) { return data_m[pos]; }
    const T& operator[](std::size_t pos) const { return data_m[pos]; }

    const T& back() const { return data_m[size_m - 1]; }

    T& back() { return data_m[size_m - 1]; }

    T& front() { return *data_m; }
    const T& front() const { return *data_m; }

    T* data() { return data_m; }

    const T* data() const { return data_m; }

    std::size_t size() const { return size_m; }

    std::size_t capacity() const { return capacity_m; }

    void push_back(const T& value);
    void push_back(T&& value);

    void emplace_back();

    void pop_back();

    void insert(std::size_t index, const T& value);

    void erase(std::size_t index);

    bool empty() const { return size_m == 0; }

    vector& swap(vector& other) noexcept;

    class Iterator {
    public:
        Iterator(T* ptr = nullptr) : ptr_m(ptr) {}

    private:
        T* ptr_m;
    };

private:
    /*
     * If this member is empty, it does not have an address, so size == 24 bytes, without
     * the attribute, it is probably (up to the compiler) 32 bytes (24 + 1 (unique addr) +
     * 7 padding)
     */
    [[no_unique_address]] Allocator alloc_m;
    T* data_m;
    std::size_t capacity_m;
    std::size_t size_m;
};

} // namespace ptorpis
