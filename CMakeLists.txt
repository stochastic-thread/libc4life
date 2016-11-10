cmake_minimum_required(VERSION 3.5)

project (c4life)
set(CMAKE_CXX_COMPILER "clang")
add_definitions(-g -std=gnu99 -Wall -Werror)

include_directories(src)

file(GLOB SOURCES src/*.c)
file(GLOB COL_SOURCES src/cols/*.c)
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${COL_SOURCES})
target_link_libraries(${PROJECT_NAME} uuid)

file(GLOB TESTS tests/*.c)
add_executable(tests ${SOURCES} ${COL_SOURCES} ${TESTS})
target_link_libraries(tests uuid)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

file(GLOB HEADERS src/*.h)
install(FILES ${HEADERS}  DESTINATION include/${PROJECT_NAME})

file(GLOB COL_HEADERS src/cols/*.h)
install(FILES ${COL_HEADERS} DESTINATION include/${PROJECT_NAME}/cols)
