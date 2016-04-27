#include "sampler-points.h"
#include <math.h>

template<int Dimension, typename T>
std::vector<T> PointSampler<Dimension, T>::random_samples(int N, T *bbox, int dim){
    std::vector<double> result;

    double maxRange = bbox[2*dim-1] - bbox[dim-1];

    for(int i = 0; i < N; i++){
        for(int d = 0; d < dim; d++){
            result.push_back(maxRange * drand48() + bbox[dim-1]);
        }
    }
    return result;
}

template<int Dimension, typename T>
std::vector<T> PointSampler<Dimension, T>::jitter_samples(int N, T *bbox, int dim){
    std::vector<T> samples;

    double maxRange = bbox[2*dim-1] - bbox[dim-1];

    if(dim == 1){
        for(int c = 0; c < N; c++){
            double x = (c + drand48())/N;
            samples.push_back(maxRange * x + bbox[dim-1]);
        }
    }
    else if(dim == 2){
        float sqN = sqrt(N);
        int iN = sqN;
        float fracN = sqN-iN;
        if( fracN != 0){
            std::cerr << "Only N with integer square root is accepted !!!" << std::endl;
            exit(-1);
        }
        int length = sqN;

        for(int r = 0; r < length; r++)
            for(int c = 0; c < length; c++){
                T x = (c + drand48())/length;
                T y = (r + drand48())/length;

                samples.push_back(maxRange * x + bbox[dim-1]);
                samples.push_back(maxRange * y + bbox[dim-1]);
            }
    }
    else if(dim == 3){
        float cbN = cbrt(N);
        int iN = cbN;
        float fracN = cbN-iN;
        if( fracN != 0){
            std::cerr << "Only N with integer cube root is accepted !!!" << std::endl;
            exit(-1);
        }
        int cblength = cbN;

        for(int r = 0; r < cblength; r++)
            for(int c = 0; c < cblength; c++){
                for(int h = 0; h < cblength; h++){
                    T x = (c + drand48())/cblength;
                    T y = (r + drand48())/cblength;
                    T z = (h + drand48())/cblength;

                    samples.push_back(maxRange * x + bbox[dim-1]);
                    samples.push_back(maxRange * y + bbox[dim-1]);
                    samples.push_back(maxRange * z + bbox[dim-1]);
                }
            }
    }
    else{
        std::cerr << "Requested dimension is not possible!!!" << std::endl;
        exit(-2);
    }
    return samples;
}


template<int Dimension, typename T>
std::vector<T> PointSampler<Dimension, T>::uniformjitter_samples(int N, T *bbox, int dim){

    std::vector<T> samples;

    T maxRange = bbox[2*dim-1] - bbox[dim-1];
    T xRandomShift = drand48();
    T yRandomShift = drand48();
    T zRandomShift = drand48();

    if(dim == 1){
        for(int c = 0; c < N; c++){
            T x = (c + xRandomShift)/N;
            samples.push_back(maxRange * x + bbox[dim-1]);
        }
    }
    else if(dim == 2){
        float sqN = sqrt(N);
        int iN = sqN;
        float fracN = sqN-iN;
        if( fracN != 0){
            std::cerr << "Only N with integer square root is accepted !!!" << std::endl;
            exit(-1);
        }
        int length = sqN;

        for(int r = 0; r < length; r++)
            for(int c = 0; c < length; c++){
                T x = (c + xRandomShift)/length;
                T y = (r + yRandomShift)/length;

                samples.push_back(maxRange * x + bbox[dim-1]);
                samples.push_back(maxRange * y + bbox[dim-1]);
            }
    }
    else if(dim == 3){
        float cbN = cbrt(N);
        int iN = cbN;
        float fracN = cbN-iN;
        if( fracN != 0){
            std::cerr << "Only N with integer cube root is accepted !!!" << std::endl;
            exit(-1);
        }
        int cblength = cbN;

        for(int r = 0; r < cblength; r++)
            for(int c = 0; c < cblength; c++){
                for(int h = 0; h < cblength; h++){
                    T x = (c + xRandomShift)/cblength;
                    T y = (r + yRandomShift)/cblength;
                    T z = (h + zRandomShift)/cblength;

                    samples.push_back(maxRange * x + bbox[dim-1]);
                    samples.push_back(maxRange * y + bbox[dim-1]);
                    samples.push_back(maxRange * z + bbox[dim-1]);
                }
            }
    }
    else{
        std::cerr << "Requested dimension is not possible!!!" << std::endl;
        exit(-2);
    }
    return samples;
}

template<int Dimension, typename T>
std::vector<T> PointSampler<Dimension, T>::regular_samples(int N, T *bbox, int dim){
    std::vector<double> samples;

    double maxRange = bbox[2*dim-1] - bbox[dim-1];

    if(dim == 1){
        for(int c = 0; c < N; c++){
            double x = (c + T(0.5))/N;
            samples.push_back(maxRange * x + bbox[dim-1]);
        }
    }
    else if(dim == 2){
        float sqN = sqrt(N);
        int iN = sqN;
        float fracN = sqN-iN;
        if( fracN != 0){
            std::cerr << "Only N with integer square root is accepted !!!" << std::endl;
            exit(-1);
        }
        int length = sqN;

        for(int r = 0; r < length; r++)
            for(int c = 0; c < length; c++){
                double x = (c + T(0.5))/length;
                double y = (r + T(0.5))/length;

                samples.push_back(maxRange * x + bbox[dim-1]);
                samples.push_back(maxRange * y + bbox[dim-1]);
            }
    }
    else if(dim == 3){
        float cbN = cbrt(N);
        int iN = cbN;
        float fracN = cbN-iN;
        if( fracN != 0){
            std::cerr << "Only N with integer cube root is accepted !!!" << std::endl;
            exit(-1);
        }
        int cblength = cbN;

        for(int r = 0; r < cblength; r++)
            for(int c = 0; c < cblength; c++){
                for(int h = 0; h < cblength; h++){
                    double x = (c + T(0.5))/cblength;
                    double y = (r + T(0.5))/cblength;
                    double z = (h + T(0.5))/cblength;

                    samples.push_back(maxRange * x + bbox[dim-1]);
                    samples.push_back(maxRange * y + bbox[dim-1]);
                    samples.push_back(maxRange * z + bbox[dim-1]);
                }
            }
    }
    else{
        std::cerr << "Requested dimension is not possible!!!" << std::endl;
        exit(-2);
    }
    return samples;
}


template std::vector<double> pointSampler2dd::random_samples(int N, double *bbox, int dim);
template std::vector<double> pointSampler2dd::jitter_samples(int N, double *bbox, int dim);
template std::vector<double> pointSampler2dd::uniformjitter_samples(int N, double *bbox, int dim);
template std::vector<double> pointSampler2dd::regular_samples(int N, double *bbox, int dim);

template std::vector<double> pointSampler3dd::random_samples(int N, double *bbox, int dim);
template std::vector<double> pointSampler3dd::jitter_samples(int N, double *bbox, int dim);
template std::vector<double> pointSampler3dd::uniformjitter_samples(int N, double *bbox, int dim);
template std::vector<double> pointSampler3dd::regular_samples(int N, double *bbox, int dim);

