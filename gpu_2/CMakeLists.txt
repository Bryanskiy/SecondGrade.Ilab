cmake_minimum_required(VERSION 3.10)
project(patterns_matching)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

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



add_executable(random_test tests/random.cpp)
target_link_libraries(
    random_test
    gpu_matching
    cpu_matching
    ${OpenCL_LIBRARY}
    Boost::program_options
)
