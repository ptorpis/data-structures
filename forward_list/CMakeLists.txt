cmake_minimum_required(VERSION 3.28)
project(customForwardList 
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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -Wuseless-cast -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wdouble-promotion -Wnull-dereference -Wshadow -Wold-style-cast")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_subdirectory(../common common)

# Header-only library
add_library(ptorpis-forward_list INTERFACE)
target_include_directories(ptorpis-forward_list INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

execute_process(COMMAND bash -c "echo $(which g++)")

# Optional: Build tests
option(BUILD_TESTS "Build tests" On)

if(BUILD_TESTS)
    enable_testing()
    
    find_package(GTest QUIET)
    
    add_executable(tests_forward_list
        tests/test_forward_list.cpp
    )
    
    target_link_libraries(tests_forward_list
        PRIVATE
        ptorpis-forward_list
        ptorpis-common
        GTest::gtest
        GTest::gtest_main
    )
    
    target_compile_options(tests_forward_list PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -g
        -O0
    )
    
    target_link_options(tests_forward_list PRIVATE
        -fsanitize=address,undefined
    )
    
    include(GoogleTest)
    gtest_discover_tests(tests_forward_list)
endif()

# Optional: Build examples
option(BUILD_EXAMPLES "Build examples" OFF)

if(BUILD_EXAMPLES)
    add_executable(basic_usage examples/basic_usage.cpp)
    target_link_libraries(basic_usage PRIVATE ptorpis-forward_list)
endif()

add_executable(forward_list src/main.cpp)
target_link_libraries(forward_list PRIVATE ptorpis-forward_list ptorpis-common)

