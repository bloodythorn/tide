# output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Set flags for GCC Build
if("${CMAKE_GENERATOR}" MATCHES "Makefiles" OR
  ("${CMAKE_GENERATOR}" MATCHES "Ninja" AND NOT WIN32))
    ## GCC Flags
    set(GCC_DEBUG_FLAGS "-std=c++17 -ggdb -Wall -pedantic -Wextra \
                         -Wfatal-errors -Wdeprecated-declarations")

    set(CMAKE_CXX_FLAGS_DEBUG ${GCC_DEBUG_FLAGS} CACHE STRING "")
    set(CMAKE_CXX_FLAGS_RELEASE "-std=c++17 -O3" CACHE STRING "")

  # Set Options
  option(CCACHE_ENABLED "Use ccache?" ON)
  option(PROFILE_ENABLED "Turn on gprof profiling (GCC Only)." OFF)
endif()

# Check for build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
    "Type of build: Debug Release" #None  RelWithDebInfo MinSizeRel
    FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    Debug Release) #None RelWithDebInfo MinSizeRel
endif()

# Profiling Flags
if(CMAKE_COMPILER_IS_GNUCC AND PROFILE_ENABLED)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif()

# ccache setup
if(CCACHE_ENABLED)
  find_program(CCACHE_FOUND NAMES ccache PATH /usr/bin)
  if(CCACHE_FOUND)
    message(STATUS "ccache found and enabled: ${CCACHE_FOUND}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_FOUND})
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_FOUND})
  else()
    message(WARNING "Could not find ccache, disabled!")
  endif()
endif()

# Build Tests
option(BUILD_TESTS "Build unit tests." ON)

# Build Examples
option(BUILD_EXAMPLES "Build example programs." ON)