cmake_minimum_required(VERSION 3.28)
project(CustomVector 
    VERSION 1.0.0
    LANGUAGES CXX
)

# Generate compile_commands.json for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags for GCC
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Header-only library
add_library(ptorpis-vec INTERFACE)
target_include_directories(ptorpis-vec INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional: Build tests
option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    enable_testing()
    
    find_package(GTest QUIET)
    
    add_executable(tests_vector
        tests/constructor.cpp
        tests/access.cpp
        tests/type.cpp
        tests/operators.cpp
        tests/reserve_push_back.cpp
        tests/general.cpp
        tests/iterators.cpp
        tests/exception_safety.cpp
    )
    
    target_link_libraries(tests_vector 
        PRIVATE 
        ptorpis-vec
        GTest::gtest
        GTest::gtest_main
    )
    
    target_compile_options(tests_vector PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -g
        -O0
    )
    
    target_link_options(tests_vector PRIVATE
        -fsanitize=address,undefined
    )
    
    include(GoogleTest)
    gtest_discover_tests(tests_vector)
endif()

# Optional: Build examples
option(BUILD_EXAMPLES "Build examples" OFF)

if(BUILD_EXAMPLES)
    add_executable(basic_usage examples/basic_usage.cpp)
    target_link_libraries(basic_usage PRIVATE ptorpis-vec)
endif()

add_executable(vector src/main.cpp)
target_link_libraries(vector PRIVATE ptorpis-vec)

