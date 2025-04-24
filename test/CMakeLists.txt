# test/CMakeLists.txt
cmake_minimum_required(VERSION 3.31)
set(CXX_STANDARD 17)

add_subdirectory(lib/googletest)
set(RADICE, lib/googletest/) # var. per includere i file di googletest a partire dalla radice di googletest
include_directories(${RADICE}/include ${RADICE})

set(TEST_SOURCE_FILES runAllTests.cpp iniParserTest.cpp) # ho solo una classe, non ha senso fare la fixture
add_executable(runAllTests ${TEST_SOURCE_FILES})
target_link_libraries(runAllTests gtest gtest_main ${CMAKE_PROJECT_NAME}_lib)