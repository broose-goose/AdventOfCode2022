
cmake_minimum_required(VERSION 3.23)

project(AdventOfCode2022_day_6)

# add_definitions(-DDEBUG)

add_executable(day_6 day_6_solver.cpp)
target_include_directories(day_6 PRIVATE ${CMAKE_SOURCE_DIR}/src)

install(TARGETS day_6 RUNTIME DESTINATION ${INSTALL_DIR})