cmake_minimum_required(VERSION 2.6)
project(eea)

include_directories(include PSA)

add_executable(eea cpp/main.cpp cpp/samples.cpp cpp/randsampgsl.cpp)
TARGET_LINK_LIBRARIES(eea gsl blas gomp)

install(TARGETS eea RUNTIME DESTINATION bin)

#LIBS *= -lCGAL -lCGAL_Core -lboost_thread-mt -lmpfr -lgsl  -lblas -lboost_program_options-mt -lcairo -lgomp -lfftw3
