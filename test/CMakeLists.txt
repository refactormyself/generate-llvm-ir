# Here is the CMake config for the test setup
# We are building a separate test (main) so that we don't have to adapt our cli app interface for testing
set(src
        common_tests.cpp
        main.cpp
)

# Get Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(common_tests ${src})

include_directories("../include")

target_link_libraries(common_tests gtest)
target_link_libraries(common_tests gtest_main)

add_test(
        NAME main_tests
        COMMAND common_tests
)
