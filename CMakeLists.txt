cmake_minimum_required(VERSION 3.0)
project(jupiter)

set(CMAKE_CXX_STANDARD 20)

include(build/CMakeLists.txt)

# Setup source files
add_subdirectory(src)
