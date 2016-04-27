#include "fft.h"
#include <iostream>

template <typename T>
void perform_dft(T *out_ptr, const T *mydata, int total_samples, int width, int height, std::string feature){

    fftw_complex *data_in;
    mycomplex *cI = new mycomplex[width*height]();

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

    double maxMagnitude =-1e5;
    double maxPower = -1e5;

    int tempr = 0, tempc = 0;
    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            /// normalize values

            double realI = data_out[r*width + c][0];
            double imagI = data_out[r*width + c][1];
            double powerI = ((realI * realI) + (imagI * imagI)) / double(total_samples);
            double magI = sqrt(powerI);

            ///Get the maximum value of the magnitude
            if(maxMagnitude < magI)
                maxMagnitude = magI;
            if(maxPower < powerI){
                tempr = r;
                tempc = c;
                maxPower = powerI;
            }

            std::complex<double> ci(realI, imagI);
            double phaseI = arg(ci) + M_PI;
            mycomplex z;
            z.r = realI;
            z.i = imagI;
            z.mag = magI;/// double(total_samples);
            z.phase = phaseI;
            z.power =powerI;
            cI[r*width+c] = z;
        }
    }

    //std::cout << maxPower << " " << tempc << " " << tempr <<  std::endl;
    double constantMagFactor = 1.0 / log10(1.0 + fabs(maxMagnitude));
    double constantPowerLogScaling = 1.0 / log10(1.0 + fabs(maxPower));

    double constantPowerScaling = 1.0 / fabs(maxPower);


    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){

            double magnitude = cI[r*width + c].mag;
            magnitude =  (magnitude < 1e-4) ? 0.0 : magnitude;
            magnitude = (constantMagFactor * log10(1.0 + magnitude));

            double power = cI[r*width + c].power;
            double powerLogScaling = constantPowerLogScaling * power;
            double powerScaling = constantPowerScaling * power; //Simple range mapping to (0,1)

            double phase = cI[r*width+c].phase;


            if(feature == "magnitude" || feature == "mag")
                out_ptr[r*width+c] = magnitude;
            else if(feature == "power-logscaled")
                out_ptr[r*width+c] = powerLogScaling;
            else if(feature == "power-scaled")
                out_ptr[r*width+c] = powerScaling;
            else if(feature == "power")
                out_ptr[r*width+c] = power;
            else if(feature == "phase")
                out_ptr[r*width+c] = phase / double(2 * M_PI);
            else if(feature == "magphase")
                out_ptr[r*width+c] = magnitude + phase;
            else{
                std::cerr << "Please specify the feature (magnitude or power) of the Fourier spectrum !!!" << std::endl;
                exit(-2);
            }
        }
    }

    swapQuadrants(out_ptr, width, height);

    fftw_destroy_plan( plan_f);
    fftw_free( data_out);
    fftw_free(data_in);
    delete [] cI;
}

template void perform_dft(double *out_ptr, const double *mydata, int total_samples, int width, int height, std::string feature);
template void perform_dft(float *out_ptr, const float *mydata, int total_samples, int width, int height, std::string feature);

