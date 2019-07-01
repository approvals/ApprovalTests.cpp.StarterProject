add_library(StarterProject.code INTERFACE)

target_include_directories(StarterProject.code
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set(TARGET StarterProject.code PROPERTY CXX_STANDARD 17)