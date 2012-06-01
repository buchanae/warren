project( warren )
option( build_tests "Build tests" OFF )

cmake_minimum_required( VERSION 2.6 )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# 
# Configure sources and includes
#
include_directories( include )
set( 
  SRCS
  src/Alignment.cpp
  src/Attributes.cpp
  src/Coverage.cpp
  src/Index.cpp
  src/GFFReader.cpp
  src/StackReader.cpp
  src/junctions.cpp
)

#
# Configure external libraries
#
find_package( bamtools REQUIRED )
include_directories( ${bamtools_INCLUDE_DIR} )

message( STATUS ${bamtools_LIBRARY} )

set(
  LIBS
  uriparser
  ${bamtools_LIBRARY}
)

#
# Build a libwarren
#
add_library( warren ${SRCS} )
target_link_libraries( warren ${LIBS} )

add_executable( make-coverage src/tools/make-coverage.cpp)
target_link_libraries( make-coverage warren ${LIBS} )

add_executable( intron-length-stats src/tools/intron-length-stats.cpp)
target_link_libraries( intron-length-stats warren ${LIBS} )

add_executable( feature-coverage src/tools/feature-coverage.cpp)
target_link_libraries( feature-coverage warren ${LIBS} )

#
# Build unit tests if 'build_tests' option is on
#
if( build_tests )

  #
  # Include these unit test files
  #
  set(
    TEST_SRCS
    tests/Alignment.cpp
    tests/Attributes.cpp
    tests/BamReader.cpp
    tests/Coverage.cpp
    tests/Feature.cpp
    tests/Index.cpp
    tests/UniquePositionIndex.cpp
    tests/GFFReader.cpp
    tests/StackReader.cpp
  )

  #
  # Copy dummy test files to build dir
  #
  add_custom_command(
    OUTPUT dummies
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_CURRENT_SOURCE_DIR}/tests/dummies"
      "${CMAKE_CURRENT_BINARY_DIR}/dummies"
      DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/dummies"
  )
  add_custom_target(TestDummies ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/dummies")

  #
  # Include googlemock libs
  #
  include_directories(
    libs/gmock/include
    libs/gmock/gtest/include
    libs/gmock
    libs/gmock/gtest
  )
  add_library(
    gmock
    libs/gmock/src/gmock-all.cc
    libs/gmock/gtest/src/gtest-all.cc
    libs/gmock/gtest/src/gtest_main.cc
  )

  #
  # Build a 'tests' executable that runs all unit tests
  #
  add_executable( tests ${SRCS} ${TEST_SRCS} )
  target_link_libraries( tests ${LIBS} gmock )

endif()
