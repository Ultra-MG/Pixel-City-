cmake_minimum_required(VERSION 3.16)
project(PixelCity LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(PixelCity ${SOURCES})

# 🔑 THIS LINE FIXES EVERYTHING
target_include_directories(PixelCity PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

find_package(SFML REQUIRED COMPONENTS System Window Graphics)
target_link_libraries(PixelCity PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
)
