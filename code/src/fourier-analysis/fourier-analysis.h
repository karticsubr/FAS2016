#ifndef SPECTRALANALYSIS_H
#define SPECTRALANALYSIS_H

#include <complex>
#include <vector>

template <typename T>
class FT{
public:

    static void continuous_fourier_spectrum(std::complex<T> *complexSpectrum, std::vector<T> &points, int width, int height, T dstep);
    static void discrete_fourier_spectrum(std::complex<T> *complexSpectrum, const T *mydata, int width, int height);

    static std::complex<T> complexcoeff_continuous_fourier_spectrum(T wx, T wy, std::vector<T> &points);

    static void power_fourier_spectrum(T *powerSpectrum, const std::complex<T>*complexSpectrum, int total_samples, int width, int height);

    static void magnitude_fourier_spectrum(T *magnitudeSpectrum, const std::complex<T>*complexSpectrum, int total_samples, int width, int height);

    static void phase_fourier_spectrum(T *phaseSpectrum, const std::complex<T>*complexSpectrum, int width, int height);

    ///Inverse using the complex spectrum
    static void inverse_discrete_fourier_transform(T *spatialSignal, const std::complex<T> *complexSpectrum, int width, int height);

    ///Inverse using the magnitude and the phase spectrum
    static void inverse_discrete_fourier_transform(T *spatialSignal, T *magnitudeSpectrum, T *phaseSpectrum, int totalSamples, int width, int height);

    static void realimag_fourier_spectrum(T *realSpectrum, T* imagSpectrum,
                                             const std::complex<T>* complexSpectrum, int width, int height);

    ///Radial / Anisotropy / Differential Tools to be added...
    static void compute_radial_mean_powerspectrum(std::string filename, T *indata, int width, int height, int nsamples);

    static void swapQuadrants(T *img, int width, int height);
};

#endif // SPECTRALANALYSIS_H
