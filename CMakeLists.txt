cmake_minimum_required(VERSION 3.15)
project(HW1 C)

set(CMAKE_C_STANDARD 99)

add_executable(HW1 main.c)
target_link_libraries(HW1 ${CMAKE_SOURCE_DIR}/libmtm.a)
