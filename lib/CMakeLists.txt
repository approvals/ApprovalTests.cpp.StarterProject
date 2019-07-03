add_library(StarterProject.lib INTERFACE)

target_include_directories(StarterProject.lib
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set(TARGET StarterProject.lib PROPERTY CXX_STANDARD 17)