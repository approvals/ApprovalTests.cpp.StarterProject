cmake_minimum_required(VERSION 3.8)
project(PlaygroundTests)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES
        main.cpp
        NewTest.cpp)
add_executable(PlaygroundTests ${SOURCE_FILES} )