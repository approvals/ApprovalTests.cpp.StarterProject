set(CODE_LIB_NAME StarterProject.code)
add_library(${CODE_LIB_NAME} INTERFACE)
target_include_directories(${CODE_LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})