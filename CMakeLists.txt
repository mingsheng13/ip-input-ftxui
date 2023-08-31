cmake_minimum_required(VERSION 3.14)
project(ftxui-ip-input)
include(cmake/ftxui.cmake)

add_library(ftxui-ip-input STATIC
        include/ftxui-ip-input/ipv4.hpp
        src/ftxui-ip-input/ipv4.cpp
        )
target_link_libraries(ftxui-ip-input PUBLIC ftxui::component)

target_include_directories(ftxui-ip-input PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
add_subdirectory(example)
