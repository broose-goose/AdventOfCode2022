
cmake_minimum_required(VERSION 3.23)

project(AdventOfCode2022_day_3)

# add_definitions(-DDEBUG)

add_executable(day_3 day_3_solver.cpp)
target_include_directories(day_3 PRIVATE ${CMAKE_SOURCE_DIR}/src)

install(TARGETS day_3 RUNTIME DESTINATION ${INSTALL_DIR})