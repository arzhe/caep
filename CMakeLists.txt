cmake_minimum_required(VERSION 3.5)

project(caep
        VERSION 1.0.0
        DESCRIPTION "Access Control Library for supporting access control models, eg: ACL, RBAC, etc"
        HOMEPAGE_URL https://github.com/arzhe/caep
        LANGUAGES CXX C
        )


if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
            "In-source build is not allowed. Please create a new sub-directory and run CMake from there."
            )
endif()


option(CAEP_BUILD_TEST "Option to build test" OFF)

# Do not print install message
if(NOT DEFINED CMAKE_INSTALL_MESSAGE)
    set(CMAKE_INSTALL_MESSAGE "ALWAYS")
endif()

if(CAEP_BUILD_TEST)
    enable_testing()
endif()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(caep)
add_subdirectory(test)
