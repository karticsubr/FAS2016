cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")
##############################################################################
project(SIGGRAPH2016-PowerSpectrum)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../build/)

set(BIN_PATH .)

#MESSAGE( STATUS ":         " ${CMAKE_BINARY_DIR})
#MESSAGE( STATUS ":         " ${CMAKE_SOURCE_DIR})
#MESSAGE( STATUS ":         " ${PROJECT_BINARY_DIR})

file(MAKE_DIRECTORY ${BIN_PATH})

set(CMAKE_BUILD_TYPE Release)  # None | Debug  | Release | RelWithDebInfo | MinSizeRel

find_package(Boost 1.54.0 COMPONENTS system filesystem date_time program_options REQUIRED)
find_package(FreeImage REQUIRED)
find_package(OpenEXR)
find_package(IlmBase)
find_package(FFTW REQUIRED)
find_package(TBB)

set(CMAKE_CXX_FLAGS " -g -Wall")

add_definitions(-std=gnu++0x)

#set(EIGEN_INCLUDE_DIR ${EIGEN_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/../ext/eigen/")

include_directories(
  ${OPENEXR_INCLUDE_DIR}
  ${ILMBASE_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
#  ${EIGEN_INCLUDE_DIR}
)

##sources
file(GLOB SOURCES
  src/io/*.cpp
  src/core/*.cpp
  src/sampler-points/*.cpp
  src/fourier-analysis/*.cpp
)

file(GLOB HEADERS
  src/io/*.h
  src/core/*.h
  src/sampler-points/*.h
  src/fourier-analysis/*.h
#  src/datatypes.h
)

function(make_exec exec_name)
        add_executable(${BIN_PATH}/${exec_name} ${SOURCES} ${HEADERS} ./src/demos/fourier-analysis-demos/${exec_name}.cxx)
        target_link_libraries(
                ${BIN_PATH}/${exec_name}
                ${FREEIMAGE_LIBRARY}
                ${OPENEXR_LIBRARIES}
                ${ILMBASE_LIBRARIES}
                ${Boost_LIBRARIES}
                ${FFTW_LIBRARIES}
                ${TBB_LIBRARIES}
        )
        install(PROGRAMS ${BIN_PATH}/${exec_name} DESTINATION bin)
endfunction()

##Create executables by calling functions defined above
make_exec(powspec-demo1)

