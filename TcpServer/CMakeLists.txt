cmake_minimum_required(VERSION 3.0.0)
project(celoxica)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -pthread)

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(celoxica ${PROJECT_SOURCES})

