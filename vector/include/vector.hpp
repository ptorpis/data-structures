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

namespace ptorpis {
/*
 * @brief Dynamic array container
 * @tparam T The type of the elements
 * @tparam Allocator The allocator type used for memory management
 */
template <typename T, typename Allocator = std::allocator<T>> class vector {
public:
    static std::size_t max_size() { return std::numeric_limits<T>::max(); }

    /*
     * Constructors
     */

    vector() : data_m(nullptr), capacity_m(0), size_m(0) {}

    /*
     * @brief count construcor: constructs vector object, allocated memory and default
     * constructs all elements.
     * @param std::size_t count: number of elements to allocate memory for and default
     * construct
     * @param const Allocator&: allocator (from template)
     */

    vector(std::size_t count, const Allocator& allocator = Allocator())
        : alloc_m(allocator), data_m(nullptr), capacity_m(count), size_m(0) {
        if (count == 0) {
            return;
        }

        data_m = std::allocator_traits<Allocator>::allocate(alloc_m, count);
        capacity_m = count;

        try {
            for (std::size_t i{}; i < count; ++i) {
                std::allocator_traits<Allocator>::construct(alloc_m, data_m + i);
                ++size_m;
            }
        } catch (const std::exception& ex) {
            for (std::size_t i{}; i < count; ++i) {
                std::allocator_traits<Allocator>::destroy(alloc_m, data_m + i);
                --size_m;
            }

            std::allocator_traits<Allocator>::deallocate(alloc_m, data_m, capacity_m);

            throw;
        }
    }

    vector(std::size_t count, const T& value);

    vector(std::initializer_list<T>);

    vector(vector& other);

    vector& operator=(const vector& other);
    vector& operator=(vector&& other) noexcept;

    ~vector() = default;

    T& at(std::size_t pos) const;

    T& operator[](std::size_t pos) const { return data_m[pos]; }

    T& back() const;
    T& front() const;

    T* data() const;

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
    Allocator alloc_m;
    T* data_m;
    std::size_t capacity_m;
    std::size_t size_m;
};

} // namespace ptorpis
