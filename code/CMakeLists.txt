cmake_minimum_required(VERSION 3.5)

project(gaussian_elimination VERSION 1.0.0 LANGUAGES CXX)

set(MAIN "main")

set(SRCS_PATH "src/*.cpp")
set(INCLUDE_DIR "include/")

file(GLOB_RECURSE SRCS ${SRCS_PATH})

add_executable(${MAIN} ${SRCS})
target_include_directories(${MAIN} PRIVATE ${INCLUDE_DIR})

set(COMPILE_AND_LINK_OPTS -O3 -g -Wall -Wextra -Wformat -fsanitize=address)
target_compile_options(${MAIN} PUBLIC ${COMPILE_AND_LINK_OPTS})
target_link_options(${MAIN} PUBLIC ${COMPILE_AND_LINK_OPTS})

target_link_libraries(${MAIN} webp)
