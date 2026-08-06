cmake_minimum_required(VERSION 3.16)
project(CTetris C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


# --------------------------------------------------
# SDL2
# --------------------------------------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)


# --------------------------------------------------
# Sources
# --------------------------------------------------
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/source_files/*.c"
)


add_executable(ctetris
    main.c
    ${GAME_SOURCES}
)


# --------------------------------------------------
# Includes
# --------------------------------------------------
target_include_directories(ctetris PRIVATE
    ${CMAKE_SOURCE_DIR}/header_files
    ${CMAKE_SOURCE_DIR}/include
)


# --------------------------------------------------
# Libraries
# --------------------------------------------------
target_link_libraries(ctetris PRIVATE
    PkgConfig::SDL2
)


# --------------------------------------------------
# Compiler warnings
# --------------------------------------------------
target_compile_options(ctetris PRIVATE
    -Wall
    -Wextra
    -Wshadow
    -Wconversion
)


option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)

if(WARNINGS_AS_ERRORS)
    target_compile_options(ctetris PRIVATE -Werror)
endif()


# --------------------------------------------------
# Address Sanitizer
# --------------------------------------------------
option(ENABLE_ASAN "Build with AddressSanitizer" ${UNIX})

if(ENABLE_ASAN)

    target_compile_options(ctetris PRIVATE
        -fsanitize=address
        -g
    )

    target_link_options(ctetris PRIVATE
        -fsanitize=address
    )

endif()


# --------------------------------------------------
# Windows DLL copy
# --------------------------------------------------
if(WIN32)

    find_file(SDL2_DLL
        NAMES SDL2.dll
        PATHS
            $ENV{MSYSTEM_PREFIX}/bin
            C:/msys64/ucrt64/bin
        REQUIRED
    )

    message(STATUS "SDL2 DLL: ${SDL2_DLL}")


    add_custom_command(TARGET ctetris POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DLL}"
            $<TARGET_FILE_DIR:ctetris>
    )

endif()


# --------------------------------------------------
# Run target
# --------------------------------------------------
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E env
        LSAN_OPTIONS=suppressions=${CMAKE_SOURCE_DIR}/asan.supp
        $<TARGET_FILE:ctetris>

    DEPENDS ctetris
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)