cmake_minimum_required(VERSION 3.6)
project(DA3_project)

set(CMAKE_C_FLAGS "-std=c99")
set(SOURCE_FILES main.c)
add_executable(DA3_project ${SOURCE_FILES})