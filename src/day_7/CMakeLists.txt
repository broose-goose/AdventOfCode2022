
cmake_minimum_required(VERSION 3.23)

project(AdventOfCode2022_day_7)

# add_definitions(-DDEBUG)

add_executable(day_7 day_7_solver.cpp)
target_include_directories(day_7 PRIVATE ${CMAKE_SOURCE_DIR}/src)

install(TARGETS day_7 RUNTIME DESTINATION ${INSTALL_DIR})