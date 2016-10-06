cmake_minimum_required(VERSION 3.6)
project(DA3_project)

set(CMAKE_C_FLAGS "-std=c99 -lm -Wall")

add_executable(standaard src/standaard/main.c src/common/check_args.c src/common/check_args.h)

add_executable(specifiek src/specifiek/main.c src/common/check_args.c src/common/check_args.h)

add_executable(generate_file test/generate_file/main.c)

add_executable(test_standaard test/test_standaard/main.c)
add_executable(test_specifiek test/test_specifiek/main.c)

