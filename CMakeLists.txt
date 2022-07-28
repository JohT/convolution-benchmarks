# The first line of any CMake project should be a call to `cmake_minimum_required`, which checks
# that the installed CMake will be able to understand the following CMakeLists, and ensures that
# CMake's behaviour is compatible with the named version. This is a standard CMake command, so more
# information can be found in the CMake docs.
cmake_minimum_required(VERSION 3.15)

# Set message log level to VERBOSE to see more details about what CMake is doing.
set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)

# Set variables with the name and the version of the project. Avoid spaces and special characters.
set(PROJECT_NAME "ConvolutionBenchmarks")
set(CURRENT_VERSION "0.1.0")

# The top-level CMakeLists.txt file for a project must contain a literal, direct call to the
# `project()` command. `project()` sets up some helpful variables that describe source/binary
# directories, and the current project version. This is a standard CMake command.
project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})

# Create a static library that contains the implementation of the project.
add_library(${PROJECT_NAME} STATIC)

# Add the directory "cmake" to the CMake search path.
# It contains additional includes for CMake. 
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Includes the environment settings from ./cmake/Environment.cmake.
include(Environment)

# Sets compile options for the target
include(CompilerOptions)

# Compile Project with C++20 Standard
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_20)
message(VERBOSE "C++ Language Standard set to C++20 for Target ${PROJECT_NAME}")

# Import dependencies

# Not used (yet). Could be interesting to see if this library could further improve performance
# Add library "pure_simd" without building it (since it needs Conan).
# As a header-only library (as in 2022) the build isn't needed anyway.
#CPMAddPackage(
#  NAME              pure_simd
#  GITHUB_REPOSITORY eatingtomatoes/pure_simd
#  GIT_TAG           95115bf04e07b79438cfa08873d42996b1d659f9
#  DOWNLOAD_ONLY     TRUE
#)

# Import pure_simd header to provide it with #include "pure_simd.hpp"
#if (pure_simd_ADDED)
#    include_directories("${PROJECT_NAME}" PRIVATE "${pure_simd_SOURCE_DIR}/include")
#    message(VERBOSE "pure_simd headers directory included for all targets: ${pure_simd_SOURCE_DIR}/include")
#endif()

# Includes the benchmark chart environment file setup from "./cmake/BenchmarkChartEnvironmentFileSetup.cmake".
include(BenchmarkChartEnvironmentFileSetup)

# Get all source files in the src directory and store them in the project_sources variable.
# GLOB_RECURSE is not recommended but used here for simplicity: https://cmake.org/cmake/help/latest/command/file.html?highlight=CONFIGURE_DEPENDS#filesystem
FILE(GLOB_RECURSE project_sources CONFIGURE_DEPENDS "source/*.cpp")
message(VERBOSE "Sources in ${CMAKE_CURRENT_SOURCE_DIR}/source:")
foreach(source ${project_sources})
   message(VERBOSE "'${source}'")
endforeach()

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. 
target_sources("${PROJECT_NAME}" PRIVATE ${project_sources})

# definitions, pick unique names that are unlikely to collide.
target_compile_definitions("${PROJECT_NAME}"
    PUBLIC
        _USE_MATH_DEFINES
)

target_link_libraries("${PROJECT_NAME}"
    PUBLIC
        ${PROJECT_NAME}::CompilerOptions
)

# Add tests
add_subdirectory(test)