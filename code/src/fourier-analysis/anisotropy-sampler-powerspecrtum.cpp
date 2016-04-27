#include "fft.h"
#include <vector>
#include <iostream>
#include <iomanip>

void compute_anisotropy_powespectrum(std::string outputfile, double *power, double* radialMeanPower, int width,
                                     int height, int nsamples){
    if(width != height){
        std::cerr << "We assume square images for radial mean power computation !!!" << std::endl;
        exit(-2);
    }
    ///Radial Power spectrum
    int halfwidth = width*0.5;

    double* anisotropyHistogram = new double[halfwidth]();
    int* histoCounter = new int[halfwidth]();
    int xcenter = halfwidth;
    int ycenter = halfwidth;
    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            double dx = xcenter-c;
            double dy = ycenter-r;
            double distance = sqrt(dx*dx+dy*dy);

            int imgIndex = r*width+c;
            int index = distance;
            if(distance > halfwidth-1)
                continue;
            else{
                double meanDeviation = power[imgIndex] - radialMeanPower[index];
                anisotropyHistogram[index] += (meanDeviation*meanDeviation);
                histoCounter[index] += 1;
            }
        }
    }

    for(int i = 0; i < halfwidth; i++)
        anisotropyHistogram[i] /= double(histoCounter[i]-1);

    std::ofstream file;
    file.open(outputfile);

    for(int i = 0; i < halfwidth; i++)
        file << i << " " << anisotropyHistogram[i] << std::endl;
    file.close();

    delete [] histoCounter;
    delete [] anisotropyHistogram;
}

