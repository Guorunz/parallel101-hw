cmake_minimum_required(VERSION 3.12)
project(hellocmake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(main main.cpp)
add_executable(main1 main1.cpp)
# add_executable(main2 main2.cpp)

#target_compile_options(main PUBLIC -ffast-math -march=native)
target_compile_options(main1 PUBLIC -ffast-math -march=native)
# target_compile_options(main2 PUBLIC -ffast-math -march=native)
