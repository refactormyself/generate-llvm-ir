cmake_minimum_required(VERSION 3.13.4)
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

find_package(LLVM REQUIRED CONFIG)

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)
include_directories(include)

# link_directories(${LLVM_BUILD_LIBRARY_DIR})
# add_definitions(${LLVM_DEFINITIONS})

# To avoid the linker error:
# undefined reference to `typeinfo for llvm::cl::GenericOptionValue'
if(NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

add_executable(gen-llvm-ir 
        main.cpp 
        generator.cpp 
)

# Find the libraries that correspond to the LLVM components
# that we have referenced and will be linking to down below
llvm_map_components_to_libnames( llvm_libs 
        # support core native orcjit
        irreader
)

target_link_libraries (gen-llvm-ir
        CLI11::CLI11
        ${llvm_libs}
        stdc++fs
)

enable_testing()
add_subdirectory(test)