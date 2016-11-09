cmake_minimum_required(VERSION 3.5)

project (c4life)
set(CMAKE_CXX_COMPILER "clang")
add_definitions(-g -std=gnu99 -Wall -Werror)

include_directories(src)

file(GLOB SOURCES src/*.c)
file(GLOB COL_SOURCES src/cols/*.c)
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${COL_SOURCES})

file(GLOB TESTS tests/*.c)
add_executable(tests ${SOURCES} ${COL_SOURCES} ${TESTS})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

file(GLOB HEADERS src/*.h)
install (FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})
