#include "fft.h"
#include <vector>
#include <iostream>
#include <iomanip>

template <typename T>
void compute_radial_mean_powerspectrum(std::string filename, T *power, int width, int height,
                                       int nsamples){

    if(width != height){
        std::cerr << "We assume square images for radial mean power computation !!!" << std::endl;
        exit(-2);
    }
    ///Radial Power spectrum
    int halfwidth = width*0.5;

    T* radialHistogram = new T[halfwidth]();
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
                radialHistogram[index] += power[imgIndex];
                histoCounter[index] += 1;
            }
        }
    }

    for(int i = 0; i < halfwidth; i++)
        radialHistogram[i] /= double(histoCounter[i]);

    std::ofstream file;
    file.open(filename);

    for(int i = 0; i < halfwidth; i++)
        file << i << " " << std::fixed << std::setprecision(15) <<  radialHistogram[i] << std::endl;
    file.close();


    delete [] histoCounter;
    delete [] radialHistogram;
}
template void compute_radial_mean_powerspectrum(std::string filename, double *power, int width, int height,int nsamples);
template void compute_radial_mean_powerspectrum(std::string filename, float *power, int width, int height,int nsamples);

template <typename T>
void compute_radial_mean_powerspectrum_integrand(std::string filename, T *power, int width, int height,
                                       int nsamples){

    if(width != height){
        std::cerr << "We assume square images for radial mean power computation !!!" << std::endl;
        exit(-2);
    }
    ///Radial Power spectrum
    int halfwidth = width*0.5;

    T* radialHistogram = new T[halfwidth]();
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
                radialHistogram[index] += power[imgIndex];
                histoCounter[index] += 1;
            }
        }
    }

    for(int i = 0; i < halfwidth; i++)
        radialHistogram[i] /= double(histoCounter[i]);

    std::ofstream file;
    file.open(filename);

    for(int i = 0; i < halfwidth; i++)
        file << i << " " << std::fixed << std::setprecision(15) <<  radialHistogram[i] << std::endl;
    file.close();


    delete [] histoCounter;
    delete [] radialHistogram;
}
template void compute_radial_mean_powerspectrum_integrand(std::string filename, double *power, int width, int height,int nsamples);
template void compute_radial_mean_powerspectrum_integrand(std::string filename, float *power, int width, int height,int nsamples);


void compute_radial_mean_powerspectrum_old(std::vector<double> &histogram, double *indata, int width, int height, int nHistoBins){

    int half_w = width * 0.5;
    int half_h = height * 0.5;
    std::vector<double> counterHisto(nHistoBins, 0);

    double maxdist = -1e10, mindist=1e10;
    std::vector<double> dists;
    std::vector<double> diffvals;
    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            double d = sqrt( ((half_w - c) * (half_w - c)) + ((half_h - r) * (half_h - r)) );

            dists.push_back(d);
            diffvals.push_back(indata[r*width+c]);

            if(maxdist < d)
                maxdist = d;
            if(mindist > d)
                mindist = d;
        }
    }

    for(int i = 0; i< dists.size(); i++){
        //std::cout << i << " " << dists[i] << " " << diffvals[i] << std::endl;
        int index = (dists[i] / maxdist) * (nHistoBins-1);
        histogram[index] += diffvals[i];
        counterHisto[index] += 1;
    }

    for(int i = 0; i < nHistoBins; i++){
        histogram[i] /= double(counterHisto[i]);
        //std::cout << i << "  "  << histogram[i] << std::endl;
    }
}

