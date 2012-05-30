project( bamtosplat )

cmake_minimum_required( VERSION 2.6 )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package( bamtools REQUIRED )
#include_directories( ${bamtools_INCLUDE_DIR} )

set(
  LIBS
  alignmentutils
  ${bamtools_LIBRARY}
)
add_executable( bam-to-splat src/main.cpp )
target_link_libraries( bam-to-splat ${LIBS} )
