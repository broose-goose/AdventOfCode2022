
cmake_minimum_required(VERSION 3.23)
project(AdventOfCode2022)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

add_definitions(-DPROJECT_DIR=${CMAKE_SOURCE_DIR})

if(WIN32)
	set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")
endif()

set(INSTALL_DIR ${CMAKE_BINARY_DIR})

add_subdirectory("src/day_1")
add_subdirectory("src/day_2")
add_subdirectory("src/day_3")
add_subdirectory("src/day_4")
add_subdirectory("src/day_5")
add_subdirectory("src/day_6")
add_subdirectory("src/day_7")
add_subdirectory("src/day_8")
