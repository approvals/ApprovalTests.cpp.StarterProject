set(TEST_NAME StarterProject.tests)
set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES
        main.cpp
        DemoTest.cpp
        NewTest.cpp
        Tutorial.cpp)
add_executable(${TEST_NAME} ${SOURCE_FILES})
target_link_libraries(${TEST_NAME} StarterProject.code ApprovalTests::ApprovalTests Catch2::Catch2)
target_compile_definitions(${TEST_NAME} PRIVATE -DAPPROVAL_TESTS_HIDE_DEPRECATED_CODE=1)
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

#if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_SIMULATE_ID MATCHES "MSVC")
#    target_compile_options(StarterProject.tests PRIVATE
#            /W4
#            /WX
#            )
#elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
#    target_compile_options(StarterProject.tests PRIVATE
#            -Wall
#            -Wextra
#            -Wpedantic
#            -Werror
#            )
#elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#    string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#    target_compile_options(StarterProject.tests PRIVATE
#            /W4
#            /WX
#            )
#endif()
