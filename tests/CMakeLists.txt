cmake_minimum_required(VERSION 3.8)
project(StarterProject.tests)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES
        main.cpp
        NewTest.cpp)
add_executable(StarterProject.tests ${SOURCE_FILES} )
add_test(NAME StarterProject.tests COMMAND StarterProject.tests)