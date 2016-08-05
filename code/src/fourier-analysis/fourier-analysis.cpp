#include "fourier-analysis.h"


template <typename T>
void FT<T>::realimag_fourier_spectrum(T *realSpectrum, T* imagSpectrum,
                                         const std::complex<T>* complexSpectrum,
                                         int width, int height){
    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            int index = r*width+c;
            T real = complexSpectrum[(r*width+c)].real();
            T imag = complexSpectrum[(r*width+c)].imag();

            realSpectrum[index] = real;
            imagSpectrum[index] = imag;
        }
    }
}

template <typename T>
void FT<T>::power_fourier_spectrum(T *powerSpectrum, const std::complex<T>* complexSpectrum, int totalSamples, int width, int height){
    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            T real = complexSpectrum[(r*width+c)].real();
            T imag = complexSpectrum[(r*width+c)].imag();

            T power = (real*real + imag*imag) / (totalSamples);

            powerSpectrum[(r*width+c)] = power;
        }
    }
}

template <typename T>
void FT<T>::magnitude_fourier_spectrum(T *magnitudeSpectrum, const std::complex<T>* complexSpectrum, int totalSamples, int width, int height){
    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            T real = complexSpectrum[(r*width+c)].real();
            T imag = complexSpectrum[(r*width+c)].imag();

            T power = (real*real + imag*imag) / (totalSamples);

            magnitudeSpectrum[(r*width+c)] = sqrt(power);
        }
    }
}

template <typename T>
void FT<T>::phase_fourier_spectrum(T* phaseSpectrum, const std::complex<T> *complexSpectrum, int width, int height){

    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            T real = complexSpectrum[(r*width+c)].real();
            T imag = complexSpectrum[(r*width+c)].imag();

            T phase = atan2(imag, real);

            phaseSpectrum[(r*width+c)] = phase;
        }
    }
}

template void FT<double>::magnitude_fourier_spectrum(double* magnitudeSpectrum, const std::complex<double>* complexSpectrum, int totalSamples, int width, int height);
template void FT<double>::power_fourier_spectrum(double* powerSpectrum, const std::complex<double>* complexSpectrum, int totalSamples, int width, int height);
template void FT<double>::phase_fourier_spectrum(double* phaseSpectrum, const std::complex<double>* complexSpectrum, int width, int height);
template void FT<double>::realimag_fourier_spectrum(double* realSpectrum, double* imagSpectrum, const std::complex<double>* complexSpectrum, int width, int height);


template void FT<float>::magnitude_fourier_spectrum(float* magnitudeSpectrum, const std::complex<float>* complexSpectrum, int totalSamples, int width, int height);
template void FT<float>::power_fourier_spectrum(float* powerSpectrum, const std::complex<float>* complexSpectrum, int totalSamples, int width, int height);
template void FT<float>::phase_fourier_spectrum(float* phaseSpectrum, const std::complex<float>* complexSpectrum, int width, int height);


