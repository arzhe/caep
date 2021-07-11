cmake_minimum_required(VERSION 3.5)

project(caep_lib)

file(GLOB_RECURSE SRC_FILES "*.cpp")

add_library(caep ${SRC_FILES})

include_directories(${CMAKE_SOURCE_DIR}/caep)

install(TARGETS caep
        DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${CMAKE_SOURCE_DIR}/caep 
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h")
