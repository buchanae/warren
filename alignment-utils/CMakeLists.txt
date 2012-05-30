project( alignmentutils )
option( build_tests "Build tests" OFF )

cmake_minimum_required( VERSION 2.6 )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package( bamtools REQUIRED )
include_directories( ${bamtools_INCLUDE_DIR} )

include_directories( include )
set( 
  SRCS
  src/Alignment.cpp
)
set(
  LIBS
  ${bamtools_LIBRARY}
)
add_library( alignmentutils SHARED ${SRCS} )
target_link_libraries( alignmentutils ${LIBS} )

install(
  TARGETS alignmentutils
  DESTINATION lib
)
install(
  FILES 
  include/Alignment.h
  include/BamPool.h
  include/BamMultiReader.h
  include/BamReader.h
  include/Splat.h
  DESTINATION include/alignmentutils
)

if( build_tests )

  enable_testing()
  set(
    TEST_SRCS
  )
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
  add_executable(
    tests
    ${TEST_SRCS}
  )
  target_link_libraries( tests alignmentutils ${LIBS} gmock )
endif()
