#include "sampler-points.h"
#include <math.h>

//Slow Dart -throwing version of Poisson Disk Sampling.
template<int Dimension, typename T>
std::vector<T> PointSampler<Dimension, T>::darthrowing_samples(int N, T *bBox, int dim, bool toroidal){

    double maxRange = bBox[2*dim-1] - bBox[dim-1];

    double radius = 0.82/sqrt(N);
    std::vector<T> buffer;
    double ex,ey;
    ex = drand48();
    ey = drand48();
    buffer.push_back(ex);
    buffer.push_back(ey);
    for(int i = 0; i < N*250; i++){
        double sx=0.0,sy=0.0;
        sx = drand48();
        sy = drand48();
        bool conflict = false;
        for(int j = 0; j < buffer.size(); j+=2){
            if(toroidal){
                for(int p = -1; p <= 1 ; p++)
                    for(int q = -1; q <= 1; q++){
                        T tempx = sx + p;
                        T tempy = sy + q;
                        T dx = tempx - buffer[j+0];
                        T dy = tempy - buffer[j+1];

                        double euclid = (dx*dx+dy*dy);
                        if(euclid < radius*radius){
                            conflict = true;
                            break;
                        }
                    }
            }
            else{  //If not toroidal
                double dx = sx - buffer[j+0];
                double dy = sy - buffer[j+1];

                double euclid = sqrt(dx*dx+dy*dy);
                if(euclid < radius){
                    conflict = true;
                    break;
                }
            }
        }
        if(!conflict){
            buffer.push_back(sx);
            buffer.push_back(sy);
        }
        if(buffer.size() == (2*N))
            break;
    }
    //std::cerr << "# darts: " << buffer.size() / 2.0 << std::endl;

    int tempN = N;
    if(buffer.size() != (2*N)){
        tempN = buffer.size()/2.0;
    }

    std::vector<T> dartSamples;

    for(int i=0; i < tempN; i++){
        T px = maxRange * buffer[2*i+0] + bBox[dim-1];
        T py = maxRange * buffer[2*i+1] + bBox[dim-1];

        dartSamples.push_back(px);
        dartSamples.push_back(py);
    }
    return dartSamples;
}

template std::vector<double> pointSampler2dd::darthrowing_samples(int N, double* bBox, int dim, bool toroidal);


