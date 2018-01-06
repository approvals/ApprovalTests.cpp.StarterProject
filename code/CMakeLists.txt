cmake_minimum_required(VERSION 3.8)
project(PlaygroundCode CXX)
set(SOURCE_FILES YourCodeGoesHere.h)
add_library(PlaygroundCode ${SOURCE_FILES} )

set_target_properties(ApprovalTests PROPERTIES LINKER_LANGUAGE CXX)