cmake_minimum_required(VERSION 3.0)
project(custom-variant)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARN_FLAGS} ${ASAN_OPTIONS} -fsanitize=address")  

add_executable(variant src/main.cpp)
