cmake_minimum_required(VERSION 3.8)
project(StarterProject.code CXX)
set(SOURCE_FILES YourCodeGoesHere.h)
add_library(StarterProject.code ${SOURCE_FILES} )

set_target_properties(StarterProject.code PROPERTIES LINKER_LANGUAGE CXX)