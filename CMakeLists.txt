project( warren )
option( build_tests "Build tests" OFF )

cmake_minimum_required( VERSION 2.6 )

include_directories( include )
set( 
  SRCS
  src/Alignment.cpp
  src/Attributes.cpp
  src/Coverage.cpp
  src/Index.cpp
  src/Reader.cpp
  src/StackReader.cpp
)
set(
  LIBS
  bamtools
)
add_library( warren ${SRCS} )
target_link_libraries( warren ${LIBS} )

#add_executable( iso_signal ${SRCS} src/main.cpp)
#target_link_libraries( iso_signal ${LIBS} )

#add_executable( intron_length_stats ${SRCS} src/intron_length_stats.cpp)
#target_link_libraries( intron_length_stats ${LIBS} )

#add_executable( feature_coverage ${SRCS} src/feature_coverage.cpp)
#target_link_libraries( feature_coverage ${LIBS} )

if( build_tests )

    enable_testing()
    set(TEST_SRCS
      tests/JunctionIndex.cpp
      tests/StackReader.cpp
      tests/Alignment.cpp
      tests/Coverage.cpp
      tests/BamReader.cpp
      tests/JunctionFilter.cpp
#      tests/helpers.cpp
    )
    add_custom_command(
        OUTPUT dummies
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/dummies"
            "${CMAKE_CURRENT_BINARY_DIR}/dummies"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/dummies"
    )
    add_custom_target(TestDummies ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/dummies")
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
        ${SRCS}
        ${TEST_SRCS}
    )
    target_link_libraries(
        tests
        ${LIBS}
        gmock
    )
endif()
