cmake_minimum_required(VERSION 3.7)
project(TripleTriadCore)


find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories (
    ${SDL2_INCLUDE_DIRS}
    /inc
    /src
)

add_executable(TripleTriadCore
${MY_SOURCES}
    src/tt.c
    src/tt_sim.c
    src/main.c
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf  SDL2_image::SDL2_image)


# add_library(stuff
#   src/stuff.c
# )

# target_include_directories(stuff PUBLIC include)
# target_link_libraries(stuff common)