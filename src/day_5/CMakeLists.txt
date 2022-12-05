
cmake_minimum_required(VERSION 3.23)

project(AdventOfCode2022_day_5)

# add_definitions(-DDEBUG)

add_executable(day_5 day_5_solver.cpp)
target_include_directories(day_5 PRIVATE ${CMAKE_SOURCE_DIR}/src)

install(TARGETS day_5 RUNTIME DESTINATION ${INSTALL_DIR})