cmake_minimum_required(VERSION 3.7)
# set (CMAKE_CXX_STANDARD 17)
project(TripleTriadCore)

include_directories (
    /inc
    /src
)

add_executable(TripleTriadCore
    src/tt.c
    src/tt_sim.c
    src/main.c
)


# add_library(stuff
#   src/stuff.c
# )

# target_include_directories(stuff PUBLIC include)
# target_link_libraries(stuff common)