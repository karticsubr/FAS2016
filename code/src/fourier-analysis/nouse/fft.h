#ifndef FFT_H
#define FFT_H

#include <fstream>
#include <complex>
#include <vector>

#include "fftw3.h"
#include "../core/constants.h"

typedef struct {
    float x;
    float y;
} point;

typedef struct {
    float r;
    float i;
    float mag;
    float phase;
    float power;
}mycomplex;

template <typename T>
void swapQuadrants(T *img, int width, int height);
void swapOctants(float *img, int width, int height, int depth);

template <typename T>
void perform_cft(T* out_ptr, std::vector<T> &points, int total_samples, int width, int height, std::string feature);
template <typename T>
void perform_dft(T *out_ptr, const T *mydata, int total_samples, int width, int height, std::string feature);

template <typename T>
std::complex<T> continous_fourier_transform(T wx, T wy, std::vector<double> &points);


template <typename T>
void compute_radial_mean_powerspectrum(std::string filename, T *indata, int width, int height, int nsamples);

template <typename T>
void compute_radial_mean_powerspectrum_integrand(std::string filename, T *indata, int width, int height, int nsamples);

void compute_anisotropy_powespectrum(std::string outputfile, double *power, double* radialMeanPower, int width,
                                     int height, int nsamples);

void differential_domain(double* histo, float* points, int num_pts, int width, int height);
#endif
