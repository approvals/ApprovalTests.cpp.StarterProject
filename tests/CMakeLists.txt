set(TEST_NAME StarterProject.tests)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES
        main.cpp
        DemoTest.cpp
        NewTest.cpp
        Tutorial.cpp)
add_executable(${TEST_NAME} ${SOURCE_FILES})
target_link_libraries(${TEST_NAME} StarterProject.code ApprovalTests::ApprovalTests Catch2::Catch2)
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
