cmake_minimum_required( VERSION 2.6 )
project( duncan )

include_directories( src )
set(SRCS
    src/Attribute.h
    src/Attribute.cpp
    src/Feature.h
    src/Feature.cpp
    src/Reader.h
    src/utils.cpp
    src/utils.h
)
set(TEST_SRCS
    tests/Feature.cpp
    tests/Attribute.cpp
)
add_executable( duncan ${SRCS} src/main.cpp )

option( build_tests "Build tests" OFF )
if( build_tests )
    enable_testing()
    add_executable( duncan_tests ${SRCS} ${TEST_SRCS} )
    target_link_libraries( duncan_tests gtest gtest_main )
    target_link_libraries( duncan_tests gmock )
    add_test( all-tests duncan_tests )
endif()
