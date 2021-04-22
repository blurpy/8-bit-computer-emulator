cmake_minimum_required(VERSION 3.1)
project(8-bit-computer-emulator)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
