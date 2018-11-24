# Min CMAKE Version, always at the top of the file.
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
# Disable in-Source build
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Set Project
project(TIDE
  VERSION 0.1.0
  DESCRIPTION "Thorn's Interactive Development Library."
  LANGUAGES CXX C
)

# Configuration
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake)

# Add Header Path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Include the source files
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Include tests
if(BUILD_TESTS)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

# Include Examples
if(BUILD_EXAMPLES)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif()

# Debug -> Needs to go at the end
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Debug.cmake)
#dumpVAR()