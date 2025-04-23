cmake_minimum_required(VERSION 3.31)
project(IniParser)

set(CMAKE_CXX_STANDARD 17)

# Libreria INI Parser
add_library(ini_parser IniParser.cpp)

# Applicazione principale
add_executable(IniParser main.cpp)
target_link_libraries(IniParser ini_parser)

# Configurazione Google Test
add_subdirectory(googletest)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/include)

# Test
add_executable(run_tests test.cpp)
target_link_libraries(run_tests ini_parser gtest gtest_main)

# Abilita Testing
enable_testing()
add_test(NAME IniParserTests COMMAND run_tests)
