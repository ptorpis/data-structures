# Data Structures 

This repo is a collection of some data structures that I have decided to reimplement in C++ from both the STL and other places from scratch. This should serve as a learning exercise for mainly me, just pushing the code here, so I have a backup and it's openly available for anyone interested.

The 2 current data structures available are `vector<T>` and `spsc_queue<T>` under the namespace `ptorpis`.

My implementation uses mainly C++20 features, but the overall standard for the project is set to 23 (mainly for `std::println`).

## `vector<T>` -- Dynamic Array

Vector is one of the most commonly used containers from the C++ STL, and conceptually, it is very simple. Store elements contiguously, and dynamically expand the capacity whenever needed.

Uses a pointer and 2 `std::size_t`s to keep track of where the start of the data is, where is the end of the data, and where is the end of the capacity.

List of public methods supported:

- Default constructor
- Count constructor
- Count & Value constructor
- Initializer list constructor
- Copy constructor
- Move constructor
- Copy assignment operator
- Move assignment operator
- Destructor
- `reserve(std::size_t capacity)`
- `at(std::size_t pos)` (both const and non-const)
- `[]` operator (both const and non-const)
- `back()` (both const and non-const)
- `front()` (both const and non-const)
- `size()`
- `capacity()`
- `push_back(const T& value)`
- `push_back(T&& value)`
- `emplace_back(Args&&... args)`
- `pop_back()`
- `clear()`
- `erase(const_iterator pos)`
- `erase(const_iterator first, const_iterator last)`
- `insert(const_iterator pos, const T& value)`
- `insert(const_iterator pos, T&& value)`
- `insert(const iterator pos, std::size_t count, const T& value)`
- `insert(const_iterator pos, InputIt first, InputIt last)`
- `insert(const_iterator pos, std::initializer_list<T> ilist)`
- `empty()`
- `shrink_to_fit()`
- Comparison operators (==, !=, >, <, >=, <=)
- Iterator methods: `begin()`, `end()`, `cbegin()`, `cend()`, `rbegin()`, `rend()`, `crbegin()`, `crend()`

## `spsc_queue` -- Lock-free Single Producer Single Consumer Queue

Very common pattern used in HFT/Quantitative Trading. This data structure allows for 2 concurrent threads (one being the producer and the other being the consumer) to pass items between each other without the use of locks.

Similarly to the `vector` implementation, the container is templated, so different types can be used, as well as support for custom allocator.

The structure uses atomic operations on the tail and head pointers when reading/writing.