cmake_minimum_required(VERSION 3.28)
project(lock-free_spsc_queue
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
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address,undefined,thread")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Header-only library
add_library(ptorpis-spscq INTERFACE)
target_include_directories(ptorpis-spscq INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional: Build tests
option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    enable_testing()
    
    find_package(GTest QUIET)
    
    add_executable(tests_spscq
        tests/single_threaded.cpp
        tests/multi_threaded.cpp
    )
    
    target_link_libraries(tests_spscq 
        PRIVATE 
        ptorpis-spscq
        GTest::gtest
        GTest::gtest_main
    )
    
    target_compile_options(tests_spscq PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -g
        -O0
    )
    
    target_link_options(tests_spscq PRIVATE
        -fsanitize=address,undefined
    )
    
    include(GoogleTest)
    gtest_discover_tests(tests_spscq)
endif()

# Optional: Build examples
option(BUILD_EXAMPLES "Build examples" OFF)

if(BUILD_EXAMPLES)
    add_executable(basic_usage examples/basic_usage.cpp)
    target_link_libraries(basic_usage PRIVATE ptorpis-spscq)
endif()

add_executable(lfspscq src/main.cpp)
target_link_libraries(lfspscq PRIVATE ptorpis-spscq)

