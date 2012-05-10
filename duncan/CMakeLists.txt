cmake_minimum_required( VERSION 2.6 )
project( duncan )

include_directories( include src )
set( 
    SRCS
    src/Attributes.cpp
    src/Feature.cpp
    src/Index.cpp
    src/Reader.cpp
)
add_library( duncan ${SRCS} )
set( LIBS uriparser )
target_link_libraries( duncan ${LIBS} )


option( build_tests "Build tests" OFF )
if( build_tests )

    enable_testing()
    set(TEST_SRCS
        tests/Attributes.cpp
        tests/Feature.cpp
        tests/Index.cpp
        tests/Reader.cpp
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
        duncan_tests
        ${SRCS}
        ${TEST_SRCS}
    )
    target_link_libraries( duncan_tests ${LIBS} gmock )
    add_test( all-tests duncan_tests )
endif()
