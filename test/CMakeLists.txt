if(CAEP_BUILD_TEST)
    set(CMAKE_CXX_STANDARD 17)

add_executable(caepgtest
               config_test.cpp
               )

target_link_libraries(caepgtest
                      gtest_main
                      caep
                      )

include(GoogleTest)

endif()
