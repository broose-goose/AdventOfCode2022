
cmake_minimum_required(VERSION 3.23)

project(AdventOfCode2022_day_1)

# add_definitions(-DDEBUG)

add_executable(day_1 day_1_solver.cpp)
target_include_directories(day_1 PRIVATE ${CMAKE_SOURCE_DIR}/src)

install(TARGETS day_1 RUNTIME DESTINATION ${INSTALL_DIR})