cmake_minimum_required(VERSION 3.16)
project(CTetris C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# On Windows, point find_package(SDL2) at this project's own lib/ folder
# (see README for setup). On Linux, libsdl2-dev already installs its own
# config into a system path CMake finds automatically -- left untouched
# here, since the bundled Windows config expects a project-root bin/
# folder that has no equivalent on Linux.
if(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}")
endif()

find_package(SDL2 REQUIRED CONFIG)

file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/source_files/*.c")

add_executable(ctetris main.c ${GAME_SOURCES})

target_include_directories(ctetris PRIVATE
    ${CMAKE_SOURCE_DIR}/header_files
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ctetris PRIVATE SDL2::SDL2main SDL2::SDL2)

target_compile_options(ctetris PRIVATE -Wall -Wextra -Wshadow -Wconversion)

option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
if(WARNINGS_AS_ERRORS)
    target_compile_options(ctetris PRIVATE -Werror)
endif()

# Classic (non -w64) MinGW does not ship libasan, so default this off on
# Windows; on by default everywhere else.
option(ENABLE_ASAN "Build with AddressSanitizer" ${UNIX})
if(ENABLE_ASAN)
    target_compile_options(ctetris PRIVATE -fsanitize=address -g)
    target_link_options(ctetris PRIVATE -fsanitize=address)
endif()

# SDL2.dll lives in bin/ (see README) -- copy it next to the built exe so
# `cmake --build build` produces something runnable without a manual step.
if(WIN32)
    add_custom_command(TARGET ctetris POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/bin/SDL2.dll" $<TARGET_FILE_DIR:ctetris>
    )
endif()

# cmake --build build --target run
# Runs the game with the LeakSanitizer suppressions in asan.supp applied.
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E env LSAN_OPTIONS=suppressions=${CMAKE_SOURCE_DIR}/asan.supp $<TARGET_FILE:ctetris>
    DEPENDS ctetris
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)