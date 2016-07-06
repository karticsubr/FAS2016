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

include_directories(include PSA)
find_package(OpenMP)

if(OPENMP_FOUND)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(eea cpp/main.cpp cpp/samples.cpp 
		cpp/integrand.cpp cpp/cmdlnparser.cpp cpp/Analyzer.cpp 
		cpp/QPIntegrand.cpp cpp/PWConstIntegrand.cpp)
TARGET_LINK_LIBRARIES(eea blas gomp CGAL gmp)

install(TARGETS eea RUNTIME DESTINATION bin)

#LIBS *= -lCGAL -lCGAL_Core -lboost_thread-mt -lmpfr -lgsl  -lblas -lboost_program_options-mt -lcairo -lgomp -lfftw3
