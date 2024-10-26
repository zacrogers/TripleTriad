cmake_minimum_required(VERSION 3.7)
# set (CMAKE_CXX_STANDARD 17)
project(TripleTriadCore)

include_directories (
    /inc
    /src
)

add_executable(TripleTriadCore
    src/tt.c
    src/test.c
    src/main.c
)
