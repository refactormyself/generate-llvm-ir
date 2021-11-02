cmake_minimum_required(VERSION 3.21)
project(gen-llvm-ir)

set(CMAKE_BUILD_WITH_INSTALL_NAME_DIR ON)
set(CMAKE_CXX_STANDARD 14 CACHE STRING "C++ standard to conform will LLVM's build")

# Get CLIUtils/CLI11
include(FetchContent)
FetchContent_Declare(
        cli11
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/CLIUtils/CLI11/archive/refs/tags/v2.1.2.zip
)
FetchContent_MakeAvailable(cli11)

# set up llvm
set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/bin)
set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/lib)

# Pass in the local LLVM installation directory (Please build LLVM from source)
set(LOCAL_LLVM_INSTALL_DIR "" CACHE PATH "LLVM installation directory")

message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Tell CMake the location of LLVMConfig.cmake so find_package is not lost :(
list(APPEND CMAKE_PREFIX_PATH "${LOCAL_LLVM_INSTALL_DIR}/lib/cmake/llvm/")

find_package(LLVM REQUIRED CONFIG)

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)
link_directories(${LLVM_BUILD_LIBRARY_DIR})
add_definitions(${LLVM_DEFINITIONS})

# To avoid the linker error:
# undefined reference to `typeinfo for llvm::cl::GenericOptionValue'
if(NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

add_executable(gen-llvm-ir main.cpp generator.cpp generator.hpp)
target_link_libraries(gen-llvm-ir CLI11::CLI11)

# To avoid undefined reference to `llvm::EnableABIBreakingChecks'
# ref: https://stackoverflow.com/a/63499379
target_link_libraries(gen-llvm-ir LLVMSupport)

## Find the libraries that correspond to the LLVM components
## that we wish to use
#llvm_map_components_to_libnames(llvm_libs support core orcjit native irreader)
#
#target_link_libraries (kal
#        ${llvm_libs}
#        )

enable_testing()
add_subdirectory(test)