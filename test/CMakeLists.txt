cmake_minimum_required(VERSION 3.13.4)
project(test)

set(src
        common_tests.cpp
        main.cpp
        include/header.h
)

set(BUILD_SHARED_LIBS on)

## compile the llvm IR
#if (EXISTS "../OutputFile.ll")
##    execute_process( COMMAND llc ../OutputFile.ll -o Output.s)
#     execute_process( COMMAND clang -shared -fPIC -o Output.so ../OutputFile.ll )
#else ()
#    message( FATAL_ERROR "Cannot find OutputFile.ll " )
#endif ()

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

add_executable(common_tests ${src}) #${CMAKE_CURRENT_BINARY_DIR}/Output.so)

target_link_libraries(common_tests gtest)
target_link_libraries(common_tests gtest_main)

add_test(
        NAME main_tests
        COMMAND common_tests
)
