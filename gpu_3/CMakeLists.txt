cmake_minimum_required(VERSION 3.10)
project(dirichlet)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-g")

find_package(OpenCL REQUIRED)

add_subdirectory(support)
add_subdirectory(dirichlet)

add_executable(main main.cpp)
target_link_libraries(
    main
    support
    dirichlet
    ${OpenCL_LIBRARY}
)
