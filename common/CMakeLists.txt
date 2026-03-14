cmake_minimum_required(VERSION 3.28)
project(ptorpis-common LANGUAGES CXX)

add_library(ptorpis-common INTERFACE)
target_include_directories(ptorpis-common INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
