cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
##############################################################################

project(FourierAnalysis)

SET(DEMOS_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/demos")

# All demos
option(homogenization "Execute homogenization DEMOS" OFF)
option(pointset "Execute pointset demos" OFF)

#configure
configure_file (
  "${PROJECT_SOURCE_DIR}/config/Config.h.in"
  "${PROJECT_BINARY_DIR}/config/Config.h"
  )
MESSAGE( STATUS ":         " ${DEMOS_DIRS})
# add the binary tree to the search path for include files
# so that we will find Config.h
include_directories ("${PROJECT_BINARY_DIR}/config")

IF(homogenization)
    include(${DEMOS_DIRS}/homogenization-demos/homogenization-demos.cmake)
ELSEIF(pointset)
    include(${DEMOS_DIRS}/pointset-demos/pointset-demos.cmake)
ELSEIF(fourier-analysis)
    include(${DEMOS_DIRS}/fourier-analysis-demos/fourier-analysis-demos.cmake)
ENDIF(homogenization)

# Executing tools always.
