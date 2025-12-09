/*
 * @file data-structures/include/vector.hpp
 * @brief Custom implementation of a dynamic array, similar to std::vector<T>
 * @author ptorpis -- Peter Torpis
 *
 * This project server as an exercise in the fundamentals of C++
 *
 * The purpose is to create a container that works similarly to std::vector
 * while learning along the way.
 */

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

namespace ptorpis {
/*
 * @brief Dynamic array container
 * @tparam T The type of the elements
 * @tparam Allocator The allocator type used for memory management
 */
template <typename T, typename Allocator = std::allocator<T>> class vector {
private:
    using alloc_traits = std::allocator_traits<Allocator>;
    static constexpr std::size_t GROWTH_FACTOR = 2;

public:
    static std::size_t max_size() { return std::numeric_limits<T>::max(); }

    /*
     * Constructors
     */

    // Default constructor with no parameters, sets data to nullptr, capacity and size to
    // 0
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

    /*
     * @brief Constructs a vector from an initializer list
     *
     * Creates a vector containing copies of the elements in the initializer list.
     *
     * @param init Initializer list to initialize the elements with
     * @param allocator Allocator to use for memory allocation
     *
     * @throws std::bad_alloc if memory allocation fails
     * @throws Any exception thrown by T's copy constructor
     *
     * @par Complexity
     * Linear in init.size()
     *
     * @par Exception Safety
     * Strong guarantee - if an exception is thrown, no resources are leaked
     */

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

    /*
     * @brief Copy constructor - constructs a vector as a copy of another
     *
     * Creates a new vector with a copy of the contents of other. The allocator
     * is obtained by calling allocator_traits::select_on_container_copy_construction
     * on other's allocator.
     *
     * @param other Another vector to be used as source to initialize the elements
     *
     * @throws std::bad_alloc if memory allocation fails
     * @throws Any exception thrown by T's copy constructor
     *
     * @par Complexity
     * Linear in other.size()
     *
     * @par Exception Safety
     * Strong guarantee - if an exception is thrown during copying, the object
     * is not constructed and no resources are leaked
     *
     * @note Creates a deep copy - the new vector is independent of the source
     * @note Only allocates memory for other.size() elements, not other.capacity()
     */
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

    /*
     * @brief Move constructor - constructs a vector by moving from another
     *
     * Constructs the vector with the contents of other using move semantics.
     * After the move, other is left in a valid but unspecified state (typically empty).
     *
     * @param other Another vector to be used as source to initialize the elements
     *
     * @throws Nothing (noexcept)
     *
     * @par Complexity
     * Constant
     *
     * @par Exception Safety
     * No-throw guarantee
     *
     * @note After the move, other is left in a valid state with size() == 0
     * @note No elements are copied - ownership of other's resources is transferred
     */
    vector(vector&& other) noexcept
        : alloc_m(std::move(other.alloc_m)), data_m(std::exchange(other.data_m, nullptr)),
          capacity_m(std::exchange(other.capacity_m, 0)),
          size_m(std::exchange(other.size_m, 0)) {}

    /**
     * @brief Copy assignment operator
     *
     * Replaces the contents with a copy of the contents of other. If
     * allocator_traits::propagate_on_container_copy_assignment is true, the
     * allocator is also copied from other.
     *
     * @param other Another vector to use as data source
     * @return Reference to *this
     *
     * @throws std::bad_alloc if memory allocation fails
     * @throws Any exception thrown by T's copy constructor
     *
     * @par Complexity
     * Linear in size() + other.size()
     *
     * @par Exception Safety
     * Basic guarantee - if an exception is thrown, *this is left in a valid
     * but unspecified state
     *
     * @note Self-assignment is handled correctly
     * @note All existing elements are destroyed before copying
     */
    vector& operator=(const vector& other) {
        if (this == &other) {
            return *this;
        }

        clear_and_deallocate_();

        if constexpr (alloc_traits::propagate_on_container_copy_assignment::value) {
            alloc_m = other.alloc_m;
        }

        if (other.size_m == 0) {
            return *this;
        }

        data_m = alloc_traits::allocate(alloc_m, other.size_m);
        capacity_m = other.size_m;

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

        return *this;
    }

    /**
     * @brief Move assignment operator
     *
     * Replaces the contents with those of other using move semantics. other is
     * left in a valid but unspecified state. If allocator_traits::
     * propagate_on_container_move_assignment is true, the allocator is also
     * moved from other.
     *
     * @param other Another vector to use as data source
     * @return Reference to *this
     *
     * @throws Nothing (noexcept)
     *
     * @par Complexity
     * Linear in size() (for destroying existing elements), plus constant
     *
     * @par Exception Safety
     * No-throw guarantee
     *
     * @note Self-assignment is handled correctly
     * @note All existing elements are destroyed before moving
     * @note After the move, other is left in a valid state with size() == 0
     */
    vector& operator=(vector&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        clear_and_deallocate_();

        if constexpr (alloc_traits::propagate_on_container_move_assignment::value) {
            alloc_m = std::move(other.alloc_m);
        }

        data_m = std::exchange(other.data_m, nullptr);
        capacity_m = std::exchange(other.capacity_m, 0);
        size_m = std::exchange(other.size_m, 0);

        return *this;
    }

    /**
     * @brief Destructor - destroys all elements and deallocates memory
     *
     * Destroys all elements in the vector and deallocates the storage.
     *
     * @throws Nothing (destructors must not throw)
     *
     * @par Complexity
     * Linear in size()
     *
     * @note If T's destructor throws, the behavior is undefined (destructors
     *       should not throw exceptions)
     */
    ~vector() {
        for (std::size_t i{}; i < size_m; ++i) {
            alloc_traits::destroy(alloc_m, data_m + i);
        }
        if (data_m) {
            alloc_traits::deallocate(alloc_m, data_m, capacity_m);
        }
    }

    void reserve(std::size_t new_capacity) {
        if (capacity_m >= new_capacity) {
            return;
        }

        T* new_data = alloc_traits::allocate(alloc_m, new_capacity);

        std::size_t moved{};
        try {
            for (; moved < size_m; ++moved) {
                alloc_traits::construct(alloc_m, new_data + moved,
                                        std::move_if_noexcept(data_m[moved]));
            }

        } catch (...) {
            for (std::size_t i{}; i < moved; ++i) {
                alloc_traits::destroy(alloc_m, new_data + i);
            }

            if (data_m) {
                alloc_traits::deallocate(alloc_m, new_data, new_capacity);
            }

            throw;
        }

        for (std::size_t i{}; i < size_m; ++i) {
            alloc_traits::destroy(alloc_m, data_m + i);
        }

        alloc_traits::deallocate(alloc_m, data_m, capacity_m);

        data_m = new_data;
        capacity_m = new_capacity;
    }

    /*
     * Access methods
     */

    /*
     * @brief Accesses element at specified location with bounds checking
     *
     * Returns a reference to the element at position pos. If pos is not
     * within the range of the container, an exception is thrown.
     *
     * @param pos Position of the element to return (zero-indexed)
     * @return Reference to the requested element
     *
     * @throws std::out_of_range if pos >= size()
     *
     * @par Complexity
     * Constant
     *
     * @see operator[] for unchecked access
     */
    T& at(std::size_t pos) {
        if (pos >= size_m) {
            throw std::out_of_range("Element accessed is out of bounds");
        }

        return data_m[pos];
    }

    /// @overload
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

    void push_back(const T& value) {
        if (size_m == capacity_m) {
            // reserve space for 1 if the vector is empty, otherwise double the capacity
            reserve(capacity_m == 0 ? 1 : capacity_m * GROWTH_FACTOR);
        }

        alloc_traits::construct(alloc_m, data_m + size_m, value);
        ++size_m;
    }

    void push_back(T&& value) {
        if (size_m == capacity_m) {
            reserve(capacity_m == 0 ? 1 : capacity_m * GROWTH_FACTOR);
        }

        alloc_traits::construct(alloc_m, data_m + size_m, std::move(value));
        ++size_m;
    }

    void emplace_back();

    /**
     * @brief Removes the last element
     *
     * Removes the last element from the container. Does not reduce capacity.
     *
     * @warning Calling pop_back on an empty container is undefined behavior
     *
     * @par Complexity
     * Constant
     *
     * @par Iterator Validity
     * Iterators and references to the last element are invalidated.
     * The end() iterator is also invalidated.
     */
    void pop_back() {
        --size_m;
        alloc_traits::destroy(alloc_m, data_m + size_m - 1);
    }

    void clear() {
        for (std::size_t i{}; i < size_m; ++i) {
            alloc_traits::destroy(alloc_m, data_m + i);
        }

        size_m = 0;
    }

    void insert(std::size_t index, const T& value);

    void erase(std::size_t index);

    bool empty() const { return size_m == 0; }

    vector& swap(vector& other) noexcept;

    bool operator==(const vector& other) const {
        if (other.size_m != size_m) {
            return false;
        }

        return std::equal(data_m, data_m + size_m, other.data_m);
    }

    bool operator!=(const vector& other) const {
        if (other.size_m != size_m) {
            return true;
        }

        return !(std::equal(data_m, data_m + size_m, other.data_m));
    }

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

    void clear_and_deallocate_() {
        if (data_m) {
            for (std::size_t i{}; i < size_m; ++i) {
                alloc_traits::destroy(alloc_m, data_m + i);
            }

            alloc_traits::deallocate(alloc_m, data_m, capacity_m);
            data_m = nullptr;
            capacity_m = 0;
            size_m = 0;
        }
    }
};

} // namespace ptorpis
