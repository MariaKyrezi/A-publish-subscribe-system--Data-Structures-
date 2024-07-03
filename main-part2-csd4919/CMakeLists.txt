project(main-part2)

cmake_minimum_required(VERSION 3.1.4)

add_library(pss STATIC pss.c)

set(CMAKE_C_STANDARD 99)

add_executable(main main.c)

target_link_libraries(main pss)
