if(CAEP_BUILD_TEST)
    set(CMAKE_CXX_STANDARD 17)

add_executable(caepgtest 
               caeper_test.cpp
               adapter_test.cpp
               matcher_test.cpp 
               util_test.cpp
               model_test.cpp
               role_manager_test.cpp
               config_test.cpp
               )

target_link_libraries(caepgtest
                      gtest
                      gtest_main
                      caep
                      )

include(GoogleTest)
#gtest_discover_tests(caepgtest)

endif()
