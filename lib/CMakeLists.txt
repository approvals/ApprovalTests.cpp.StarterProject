set(LIB_NAME StarterProject.lib)
add_library(${LIB_NAME} INTERFACE)
target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(catch2)
target_link_libraries(${LIB_NAME} INTERFACE Catch2::Catch2)
