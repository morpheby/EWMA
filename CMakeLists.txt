cmake_minimum_required(VERSION 3.16)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(EWMA CXX)
endif()

add_library(ewma STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Ewma.cpp"
)
target_include_directories(ewma PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
