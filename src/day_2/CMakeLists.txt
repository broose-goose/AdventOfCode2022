
cmake_minimum_required(VERSION 3.23)

project(AdventOfCode2022_day_2)

# add_definitions(-DDEBUG)

add_executable(day_2 day_2_solver.cpp)
target_include_directories(day_2 PRIVATE ${CMAKE_SOURCE_DIR}/src)

install(TARGETS day_2 RUNTIME DESTINATION ${INSTALL_DIR})