#include "fourier-analysis.h"
#include "../core/constants.h"
#include <math.h>
#include "fftw3.h"

template <typename T>
void FT<T>::discrete_fourier_spectrum(std::complex<T> *complexSpectrum, const T *mydata, int width, int height){

    fftw_complex *data_in;

    ///initialize arrays for fftw operations
    data_in = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * width* height);

    ///Store data in the real component of the data_in array.
    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            data_in[r*width+c][0] = mydata[r*width+c];
            data_in[r*width+c][1] = 0.0;
        }
    }

    fftw_complex    *data_out;
    fftw_plan       plan_f;

    data_out = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height );

    plan_f = fftw_plan_dft_2d(width, height, data_in, data_out,  FFTW_FORWARD,  FFTW_ESTIMATE);

    fftw_execute( plan_f);

    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            /// normalize values
            int index = r*width+c;

            complexSpectrum[index].real(data_out[r*width + c][0]);
            complexSpectrum[index].imag(data_out[r*width + c][1]);
        }
    }

    fftw_destroy_plan( plan_f);
    fftw_free( data_out);
    fftw_free(data_in);
}



template <typename T>
void FT<T>::inverse_discrete_fourier_transform(T *spatialSignal, const std::complex<T>*complexSpectrum, int width, int height){

    fftw_complex    *data_out, *inverse_out;
    fftw_plan       plan_b;

    data_out = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height );
    inverse_out = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height );

    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            /// normalize values
            int index = r*width+c;

            data_out[r*width + c][0] = complexSpectrum[index].real();
            data_out[r*width + c][1] = complexSpectrum[index].imag();
        }
    }
    //#####################################################

    ///Backward (Inverse) Fourier Transform
    plan_b = fftw_plan_dft_2d(width, height, data_out, inverse_out,  FFTW_BACKWARD,  FFTW_ESTIMATE);
    fftw_execute(plan_b);

    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            int index = r*width+c;
            spatialSignal[index] = inverse_out[index][0] / double(width*height);
        }
    }
    //#####################################################
    fftw_destroy_plan( plan_b);
    fftw_free( data_out);
    fftw_free(inverse_out);
}

template <typename T>
void FT<T>::inverse_discrete_fourier_transform(T *spatialSignal, T *magnitudeSpectrum, T *phaseSpectrum, int totalSamples, int width, int height){
    fftw_complex    *data_out, *inverse_out;
    fftw_plan       plan_b;

    data_out = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height );
    inverse_out = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height );

    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            /// normalize values
            int index = r*width+c;

            data_out[r*width + c][0] = magnitudeSpectrum[index] * sqrt(totalSamples) * cos(phaseSpectrum[index]);
            data_out[r*width + c][1] = magnitudeSpectrum[index] * sqrt(totalSamples) * sin(phaseSpectrum[index]);
        }
    }
    //#####################################################

    ///Backward (Inverse) Fourier Transform
    plan_b = fftw_plan_dft_2d(width, height, data_out, inverse_out,  FFTW_BACKWARD,  FFTW_ESTIMATE);
    fftw_execute(plan_b);

    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            int index = r*width+c;
            spatialSignal[index] = inverse_out[index][0] / double(width*height);
        }
    }
    //#####################################################
    fftw_destroy_plan( plan_b);
    fftw_free( data_out);
    fftw_free(inverse_out);
}

//template <typename T>
//void FT<T>::realimag_squaremean_spectrum(T* result, const std::complex<T> *complexSpectrum,
//                                          int totalSamples, int width, int height){

//    double real = complexSpectrum->real();
//    double imag = complexSpectrum->imag();

//    double squaremean =

//}

template void FT<double>::discrete_fourier_spectrum(std::complex<double>* complexSpectrum, const double* mydata, int width, int height);
template void FT<double>::inverse_discrete_fourier_transform(double* spatialSignal, const std::complex<double>* complexSpectrum, int width, int height);
template void FT<double>::inverse_discrete_fourier_transform(double *spatialSignal, double *magnitudeSpectrum, double *phaseSpectrum, int totalSamples, int width, int height);

template void FT<float>::discrete_fourier_spectrum(std::complex<float>* complexSpectrum, const float* mydata, int width, int height);
template void FT<float>::inverse_discrete_fourier_transform(float* spatialSignal, const std::complex<float>* complexSpectrum, int width, int height);
template void FT<float>::inverse_discrete_fourier_transform(float *spatialSignal, float *magnitudeSpectrum, float *phaseSpectrum, int totalSamples, int width, int height);

