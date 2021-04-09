cmake_minimum_required(VERSION 3.10)
project(patterns_matching)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

add_subdirectory(matrix)


add_executable(main main.cpp)
target_link_libraries(
    main 
    matrix
)
