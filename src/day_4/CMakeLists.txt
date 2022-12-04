
cmake_minimum_required(VERSION 3.23)

project(AdventOfCode2022_day_4)

# add_definitions(-DDEBUG)

add_executable(day_4 day_4_solver.cpp)
target_include_directories(day_4 PRIVATE ${CMAKE_SOURCE_DIR}/src)

install(TARGETS day_4 RUNTIME DESTINATION ${INSTALL_DIR})