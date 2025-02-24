cmake_minimum_required(VERSION 3.27)
project(modelo C)

set(CMAKE_C_STANDARD 17)
include_directories(include)

add_executable(modelo main.c
        dado_estacao.c
        dado_estacao.h)
target_link_libraries(modelo ${CMAKE_SOURCE_DIR}/lib/libprg.a m)

