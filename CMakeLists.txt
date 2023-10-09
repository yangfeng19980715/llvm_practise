cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(llvm_Kaleidoscope LANGUAGES CXX)

add_definitions("-Wall -g")

add_executable(a.out
    # ch_1/lexer.cpp
    ch_2/parser_and_ast.cpp
)

include_directories(
    .
)
