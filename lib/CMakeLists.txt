if(NOT TARGET ApprovalTests)
    set(LIB_NAME ApprovalTests)
    add_library(${LIB_NAME} INTERFACE)
    target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    add_library(${LIB_NAME}::${LIB_NAME} ALIAS ${LIB_NAME})
endif()

add_subdirectory(catch2v3)
