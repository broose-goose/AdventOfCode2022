
cmake_minimum_required(VERSION 3.23)

project(AdventOfCode2022_day_8)

# add_definitions(-DDEBUG)

add_executable(day_8 day_8_solver.cpp)
target_include_directories(day_8 PRIVATE ${CMAKE_SOURCE_DIR}/src)

install(TARGETS day_8 RUNTIME DESTINATION ${INSTALL_DIR})