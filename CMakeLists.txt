cmake_minimum_required(VERSION 3.31)
project(lab_programmazione VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## Direttive di compilazione
#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    add_compile_options(-Wall -Wextra -Wpedantic -g)
#else()
#    add_compile_options(-O3)
#endif()

set(SOURCE_FILES iniParser.cpp iniParser.h)
add_executable(${CMAKE_PROJECT_NAME} main.cpp)
add_library(${CMAKE_PROJECT_NAME}_lib ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_lib)