cmake_minimum_required(VERSION 2.6)
project(eea)

include_directories(include PSA)
find_package(OpenMP)

# if(OPENMP_FOUND)
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
# endif()

add_executable(eea cpp/main.cpp cpp/samples.cpp cpp/randsampgsl.cpp cpp/integrand.cpp cpp/cmdlnparser.cpp cpp/Analyzer.cpp)
TARGET_LINK_LIBRARIES(eea gsl blas gomp CGAL gmp)

install(TARGETS eea RUNTIME DESTINATION bin)

#LIBS *= -lCGAL -lCGAL_Core -lboost_thread-mt -lmpfr -lgsl  -lblas -lboost_program_options-mt -lcairo -lgomp -lfftw3
