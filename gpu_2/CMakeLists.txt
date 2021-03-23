cmake_minimum_required(VERSION 3.10)
project(patterns_matching)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCL REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

add_subdirectory(gpu_matching)
add_subdirectory(cpu_matching)

add_executable(main main.cpp)
target_link_libraries(
    main 
    gpu_matching
    cpu_matching
    ${OpenCL_LIBRARY}
    Boost::program_options
)

