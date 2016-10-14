cmake_minimum_required(VERSION 3.6)
project(DA3_project)

set(CMAKE_C_FLAGS "-std=c99 -lm -Wall")

add_executable(standaard src/standaard/main.c src/common/check_args.c src/common/check_args.h src/standaard/encoder.c src/standaard/encoder.h src/common/constanst.h src/standaard/circular_string.c src/standaard/circular_string.h src/standaard/burrows_wheeler.c src/standaard/burrows_wheeler.h src/standaard/move_to_front.c src/standaard/move_to_front.h src/common/uchar.h)

add_executable(specifiek src/specifiek/main.c src/common/check_args.c src/common/check_args.h src/common/constanst.h src/common/uchar.h)

add_executable(test_standaard test/test_standaard/main.c test/test_common/testmacro.h test/test_standaard/test_circular_string.c test/test_standaard/test_circular_string.c test/test_standaard/test_circular_string.c test/test_standaard/test_circular_string.h src/standaard/circular_string.c src/standaard/circular_string.h test/test_standaard/test_burrows_wheeler.c test/test_standaard/test_burrows_wheeler.h src/standaard/burrows_wheeler.c src/standaard/burrows_wheeler.h test/test_standaard/test_move_to_front.c test/test_standaard/test_move_to_front.h src/standaard/move_to_front.c src/standaard/move_to_front.h src/common/uchar.h)
add_executable(test_specifiek test/test_specifiek/main.c test/test_common/testmacro.h src/common/uchar.h)

