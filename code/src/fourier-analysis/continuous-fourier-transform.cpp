#include "fourier-analysis.h"
#include "./../core/tbb_includes.h"
#include "./../core/constants.h"

#include <iostream>

template <typename T>
void FT<T>::continuous_fourier_spectrum_parallel(std::complex<T> *complexSpectrum,
                                                 std::vector<T> &points, int width,
                                                 int height, T dstep){

    int halfwidth = width * 0.5;
    int halfheight = height * 0.5;
    int npoints = points.size() * 0.5;

    //tbb::tick_count t0 = tbb::tick_count::now();
    tbb::task_scheduler_init init(8);
    tbb::parallel_for(
        tbb::blocked_range2d<int>(0,width, 16, 0, height, 16),
        [=](const tbb::blocked_range2d<int>& imgblock ) {
            for( int row = imgblock.rows().begin(); row != imgblock.rows().end(); ++row ){
                for( int col = imgblock.cols().begin(); col != imgblock.cols().end(); ++col ) {
                    T fx = 0.f, fy = 0.f;
                    T wx = (col-halfwidth)*dstep;
                    T wy = (row-halfheight)*dstep;

                    for (int i = 0; i < npoints; ++i) {
                        T exp = -twopi * (wx * points[2*i+0] + wy * points[2*i+1]);
                        fx += cos(exp);
                        fy += sin(exp);
                    }

                    complexSpectrum[row*width+col].real(fx); ///real part
                    complexSpectrum[row*width+col].imag(fy);  ///imaginary part
                }
            }
    }
    );
}

template void FT<double>::continuous_fourier_spectrum_parallel(std::complex<double> *complexSpectrum,
                                                          std::vector<double> &points, int width,
                                                          int height, double dstep);

template <typename T>
std::complex<T> FT<T>::complexcoeff_continuous_fourier_spectrum(T wx, T wy, std::vector<T> &points){
    T fx = 0.f, fy = 0.f;

    int npoints = points.size() * 0.5;

    for (int i = 0; i < npoints; ++i) {
        T exp = -twopi * (wx * points[2*i+0] + wy * points[2*i+1]);
        fx += cos(exp);
        fy += sin(exp);
    }
    std::complex<T> result(fx, fy);
    return result;
}

template std::complex<double> FT<double>::complexcoeff_continuous_fourier_spectrum(double wx, double wy, std::vector<double> &points);
template std::complex<float> FT<float>::complexcoeff_continuous_fourier_spectrum(float wx, float wy, std::vector<float> &points);

/*
template <typename T>
T* inverse_fourier_spectrum_parallel(T* result, T* complexSpectrum, int total_samples, int width,
                          int height, T dstep){

    int halfwidth = width * 0.5;
    int halfheight = height * 0.5;
    int npoints = points.size() * 0.5;

    T* complexSpectrum = new T[2*width*height] ();

    //tbb::tick_count t0 = tbb::tick_count::now();
    tbb::task_scheduler_init init(8);
    tbb::parallel_for(
        tbb::blocked_range2d<int>(0,width, 16, 0, height, 16),
        [=](const tbb::blocked_range2d<int>& imgblock ) {
            for( int row = imgblock.rows().begin(); row != imgblock.rows().end(); ++row ){
                for( int col = imgblock.cols().begin(); col != imgblock.cols().end(); ++col ) {
                    T fx = 0.f, fy = 0.f;
                    T wx = (col-halfwidth)*dstep;
                    T wy = (row-halfheight)*dstep;

                    for(int r = 0; r < width; r++){
                        for(int c = 0; c < height; c++){
                            T kx = (c-halfwidth)*dstep;
                            T ky = (r-halfheight)*dstep;
                            T exp = twopi * (wx * kx + wy * ky);
                            fx += complexSpectrum[2*(r*width+c)+0] * cos(exp);
                            fy += complexSpectrum[2*(r*width+c)+1] * sin(exp);
                        }
                    }

                    result[2*(row*width+col)+0] += fx; ///real part
                    result[2*(row*width+col)+1] += fy;  ///imaginary part

                }
            }
    }
    );

    return result;
}
*/


