cmake_minimum_required( VERSION 2.8 )
project ( cloudPlatformToolBox )

set( CMAKE_CXX_COMPILER "g++" )
set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3" )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )

############### dependencies ######################

############### source and test ######################
include_directories( ${PROJECT_SOURCE_DIR}/include )
add_subdirectory( src )
add_subdirectory( test )