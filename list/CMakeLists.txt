cmake_minimum_required(VERSION 3.28)
project(CustomList 
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
add_library(ptorpis-list INTERFACE)
target_include_directories(ptorpis-list INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional: Build tests
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    
    find_package(GTest QUIET)
    
    add_executable(test_list
    )
    
    target_link_libraries(test_list 
        PRIVATE 
        ptorpis-list
        GTest::gtest
        GTest::gtest_main
    )
    
    target_compile_options(test_list PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -g
        -O0
    )
    
    target_link_options(test_list PRIVATE
        -fsanitize=address,undefined
    )
    
    include(GoogleTest)
    gtest_discover_tests(test_list)
endif()

# Optional: Build examples
option(BUILD_EXAMPLES "Build examples" OFF)

if(BUILD_EXAMPLES)
    add_executable(basic_usage examples/basic_usage.cpp)
    target_link_libraries(basic_usage PRIVATE ptorpis-list)
endif()

add_executable(list src/main.cpp)
target_link_libraries(list PRIVATE ptorpis-list)

