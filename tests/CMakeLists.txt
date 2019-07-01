set(SOURCE_FILES
        main.cpp
		DemoTest.cpp
        NewTest.cpp)

add_executable(StarterProject.tests ${SOURCE_FILES} )

target_link_libraries(StarterProject.tests StarterProject.code StarterProject.lib)

set(TARGET StarterProject.tests PROPERTY CXX_STANDARD 17)

add_test(NAME StarterProject.tests COMMAND StarterProject.tests)