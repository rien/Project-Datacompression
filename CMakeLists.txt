cmake_minimum_required(VERSION 3.6)
project(DA3_project)

set(CMAKE_C_FLAGS "-std=c99 -lm -Wall")

set(builddir "${CMAKE_CURRENT_SOURCE_DIR}/out/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${builddir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${buildddir}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${builddir}")

FILE(GLOB common src/common/*)

FILE(GLOB standaard src/standaard/*)
FILE(GLOB specifiek src/specifiek/*)
FILE(GLOB test_common test/test_common/*)
FILE(GLOB test_standaard test/test_standaard/*)
FILE(GLOB test_specifiek test/test_specifiek/*)

add_executable(standaard src/main_standaard.c ${standaard} ${common})

add_executable(specifiek src/main_specifiek.c ${specifiek} ${common})

add_executable(test_standaard ${standaard} ${common} ${test_common} ${test_standaard})

add_executable(test_specifiek ${specifiek} ${common} ${test_common} ${test_specifiek})

