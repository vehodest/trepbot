cmake_minimum_required(VERSION 3.5)
project(trepbot_tests)

#Note that googlemock target already builds googletest 
add_subdirectory(googlemock)
enable_testing()

include_directories(${gtest_SOURCE_DIR}/include)

add_executable(trepbot_tests blanktest.cpp)

target_link_libraries(trepbot_tests gtest gtest_main)
target_link_libraries(trepbot_tests pthread)

