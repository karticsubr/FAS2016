cmake_minimum_required(VERSION 2.6)
project(eea)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


find_package(OpenMP)
if(OPENMP_FOUND)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

SET(ILMBASE_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)
SET(OPENEXR_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)
SET(OPENEXR_BUILD_SHARED_LIBS    OFF CACHE BOOL " " FORCE)
SET(ILMBASE_BUILD_SHARED_LIBS    OFF CACHE BOOL " " FORCE)
ADD_SUBDIRECTORY(ext/openexr)

SET_PROPERTY(TARGET IexMath eLut toFloat b44ExpLogTable dwaLookups CopyIlmBaseLibs IlmThread Half Iex Imath IlmImf PROPERTY FOLDER "dependencies")
INCLUDE_DIRECTORIES (
ext/openexr/IlmBase/Imath
ext/openexr/IlmBase/Half
ext/openexr/IlmBase/Iex
ext/openexr/OpenEXR/IlmImf
${CMAKE_BINARY_DIR}/ext/openexr/IlmBase/config
${CMAKE_BINARY_DIR}/ext/openexr/OpenEXR/config
)

INCLUDE_DIRECTORIES (
/usr/local/include
)

SET(OPENEXR_LIBS IlmImf Imath Half)

include_directories(include)

FILE ( GLOB SOURCE
cpp/*.cpp
)

FILE ( GLOB HEADERS
include/*.h

)

add_executable(eea ${SOURCE} ${HEADERS})

if(APPLE)
TARGET_LINK_LIBRARIES(eea blas CGAL gmp ${OPENEXR_LIBS})
else()
TARGET_LINK_LIBRARIES(eea blas gomp CGAL gmp ${OPENEXR_LIBS})
endif(APPLE)

install(TARGETS eea RUNTIME DESTINATION bin)
