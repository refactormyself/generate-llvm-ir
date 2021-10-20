cmake_minimum_required(VERSION 3.21)
project(cli_app_basic)

set(CMAKE_CXX_STANDARD 17)

# Get CLIUtils/CLI11
include(FetchContent)
FetchContent_Declare(
        cli11
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/CLIUtils/CLI11/archive/refs/tags/v2.1.2.zip
)
FetchContent_MakeAvailable(cli11)

add_executable(cli_app_basic main.cpp)
target_link_libraries(cli_app_basic CLI11::CLI11)

enable_testing()
add_subdirectory(test)