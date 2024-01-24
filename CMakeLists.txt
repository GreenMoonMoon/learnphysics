cmake_minimum_required(VERSION 3.27)
project(learnphysics C)

set(CMAKE_C_STANDARD 23)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
        GIT_TAG 5.0
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

add_executable(learnphysics src/main.c
        src/rigidbody.c
        src/rigidbody.h)
target_link_libraries(learnphysics PRIVATE raylib)