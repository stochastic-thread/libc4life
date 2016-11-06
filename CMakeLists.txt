cmake_minimum_required(VERSION 3.5)

project (c4life)
set(CMAKE_CXX_COMPILER "clang")
add_definitions(-g -std=gnu99 -Wall -Werror)

include_directories(src)

file(GLOB SOURCES src/*.c)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

file(GLOB TESTS tests/*.c)
add_executable(tests ${SOURCES} ${TESTS})

install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})

file(GLOB HEADERS src/*.h)
install (FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})
