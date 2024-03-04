cmake_minimum_required(VERSION 3.27)
project(learnphysics C)

set(CMAKE_C_STANDARD 23)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
        GIT_PROGRESS TRUE
)

FetchContent_Declare(
        cglm
        GIT_REPOSITORY https://github.com/recp/cglm.git
        GIT_TAG v0.9.2
)

FetchContent_Declare(
        flecs
        GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
        GIT_TAG v3.2.11
)

FetchContent_MakeAvailable(raylib flecs)
# Make CGLM available
if(NOT cglm_POPULATED)
    FetchContent_Populate(cglm)
    include_directories(${cglm_SOURCE_DIR}/include)
endif ()

# Make flecs headers available
include_directories(${flecs_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/src/
                    ${PROJECT_SOURCE_DIR}/src/external)

add_executable(learnphysics src/main.c
        src/rigidbody.c
        src/collision.c
        src/geometry_utils.c
        src/particles.c
        src/primitives.c
        src/tools.c
        src/resources.c
        src/scene.c

        src/graphic/draw_primitive.c

        src/raylib_utils.c
        src/gizmos.c)
target_link_libraries(learnphysics PRIVATE raylib flecs::flecs_static)

add_subdirectory(tests)