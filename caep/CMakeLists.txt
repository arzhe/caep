set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILES "*.cpp")

add_library(caep ${SRC_FILES})

include_directories(${CMAKE_SOURCE_DIR}/caep)

install(TARGETS caep
        DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${CMAKE_SOURCE_DIR}/caep 
        DESTINATION include 
        FILES_MATCHING PATTERN "*.h")
