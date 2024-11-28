cmake_minimum_required(VERSION 3.5)

project(gaussian_elimination VERSION 1.0.0 LANGUAGES CXX)

if(NOT DEFINED DO_BUILD_FOR_PLUGIN)
    set(DO_BUILD_FOR_PLUGIN FALSE)
endif()


set(SRCS_PATH "src/lib/*.cpp")
set(INCLUDE_DIR "include/" "/usr/include/freetype2")

file(GLOB_RECURSE SRCS ${SRCS_PATH})

set(LIB "asciify")

add_library(${LIB} SHARED ${SRCS} "src/c_glue.cpp")
target_include_directories(${LIB} PRIVATE ${INCLUDE_DIR})

target_link_libraries(${LIB} webp jpeg freetype)

if(NOT DO_BUILD_FOR_PLUGIN)
    set(MAIN "main")

    add_executable(${MAIN} "src/main.cpp")
    target_include_directories(${MAIN} PRIVATE "include/")

    set(COMPILE_AND_LINK_OPTS -Og -g -Wall -Wextra -Wformat -fsanitize=address)
    target_compile_options(${MAIN} PRIVATE ${COMPILE_AND_LINK_OPTS})
    target_link_options(${MAIN} PRIVATE ${COMPILE_AND_LINK_OPTS})

    target_link_libraries(${MAIN} ${LIB})
endif()

