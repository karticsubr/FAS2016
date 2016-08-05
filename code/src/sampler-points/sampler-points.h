//
//  sampler.h
//
//
//  Created by Gurprit Singh on 07/12/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _sampler_h
#define _sampler_h

#include <iostream>
#include <vector>
//#include "../datatypes.h"

template <int Dimension, typename T>
class PointSampler{
public:
    static std::vector<T> random_samples(int N, T *bbox, int dim=Dimension);
    static std::vector<T> jitter_samples(int N, T *bbox, int dim=Dimension);
    static std::vector<T> nrooks_samples(int N, T *bbox, int dim=Dimension, bool shuffle=true);
    static std::vector<T> regular_samples(int N, T *bbox, int dim=Dimension);
    static std::vector<T> uniformjitter_samples(int N, T *bbox, int dim=Dimension);
    static std::vector<T> multijitter_samples(int N, T *bbox, int dim=2, bool shuffle=true);
    static std::vector<T> darthrowing_samples(int N, T *bBox, int dim=Dimension, bool toroidal=true);
    static std::vector<T> bluenoise_samples(int N, T* bBox, std::string samplingpattern, std::string filepath="", int dim=2);
    static std::vector<T> halton_samples(int N, T *bbox, int ucomponent, int vcomponent, bool faure=true);
    static std::vector<T> halton_sequence_samples(const int& N, T *bbox, int dim=Dimension, bool shuffle=true);
    static std::vector<T> hammersley_sequence_samples(const int& N, T *bbox, int dim=Dimension, bool shuffle=true);

};

template<typename T>
std::vector<T> homogenize_pointsamples2d(std::vector<T> &vec, T* bbox);

//std::vector<double> homogenize_pointsamples2d(std::vector<double> &vec, double* domain);

std::vector<double> homogenize_pointsamples(std::vector<double> &vec, double* bbox, int nDims);


typedef PointSampler<2, double> pointSampler2dd;
typedef PointSampler<3, double> pointSampler3dd;

#endif
