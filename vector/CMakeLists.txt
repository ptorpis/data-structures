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
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    
    # Find Google Test
    find_package(GTest QUIET)
    
    if(NOT GTest_FOUND)
        # Fetch Google Test if not found
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1
        )
        FetchContent_MakeAvailable(googletest)
    endif()
    
    # Test executable
    add_executable(vector_tests
        tests/test_main.cpp
        tests/test_constructors.cpp
        # tests/test_element_access.cpp
        # tests/test_iterators.cpp
        # tests/test_capacity.cpp
        # tests/test_modifiers.cpp
    )
    
    target_link_libraries(vector_tests 
        PRIVATE 
        custom_vector
        GTest::gtest
        GTest::gtest_main
    )
    
    # Register tests with CTest
    include(GoogleTest)
    gtest_discover_tests(vector_tests)
endif()

# Optional: Build examples
option(BUILD_EXAMPLES "Build examples" OFF)

if(BUILD_EXAMPLES)
    add_executable(basic_usage examples/basic_usage.cpp)
    target_link_libraries(basic_usage PRIVATE ptorpis-vec)
endif()

add_executable(vector src/main.cpp)
target_link_libraries(vector PRIVATE ptorpis-vec)

