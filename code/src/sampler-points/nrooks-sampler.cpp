#include "sampler-points.h"

template<int Dimension, typename T>
std::vector<T> PointSampler<Dimension, T>::nrooks_samples(int N, T *bbox, int dim, bool shuffle){

    std::vector<T> samples(2*N, 0.0);

    double maxRange = bbox[2*dim-1] - bbox[dim-1];

    if(dim == 1){
        for(int c = 0; c < N; c++){
            double x = (c + drand48())/N;
            samples.push_back(maxRange * x + bbox[dim-1]);
        }
    }
    else if(dim == 2){
//        float sqN = sqrt(N);
//        int iN = sqN;
//        float fracN = sqN-iN;
//        if( fracN != 0){
//            std::cerr << "Only N with integer square root is accepted !!!" << std::endl;
//            exit(-1);
//        }
//        int length = sqN;

        std::vector<double> bins(N,0.0);

        for(int r = 0; r < N; r++){
            T x = (r + drand48())/N;
//            T x = (r)/double(N);
            bins[r] = x;
            //samples[2*index] = x;
        }

        if(shuffle)
            std::random_shuffle(bins.begin(), bins.end() );

        for(int r = 0; r < N; r++){
            samples[2*r] = bins[r];
            ///reinitialise the bins to zeros
            bins[r] = 0.0;
        }

        for(int c = 0; c < N; c++){
            T y = (c+drand48())/double(N);
            bins[c] = y;
        }

//        if(shuffle)
//            std::random_shuffle(bins.begin(), bins.end() );

        for(int c = 0; c < N; c++){
            samples[2*c+1] = bins[c];
        }
    }
    else if(dim == 3){
//        float cbN = cbrt(N);
//        int iN = cbN;
//        float fracN = cbN-iN;
//        if( fracN != 0){
//            std::cerr << "Only N with integer cube root is accepted !!!" << std::endl;
//            exit(-1);
//        }
//        int cblength = cbN;

//        for(int r = 0; r < cblength; r++)
//            for(int c = 0; c < cblength; c++){
//                for(int h = 0; h < cblength; h++){
//                    T x = (c + drand48())/cblength;
//                    T y = (r + drand48())/cblength;
//                    T z = (h + drand48())/cblength;

//                    samples.push_back(maxRange * x + bbox[dim-1]);
//                    samples.push_back(maxRange * y + bbox[dim-1]);
//                    samples.push_back(maxRange * z + bbox[dim-1]);
//                }
//            }
    }
    else{
        std::cerr << "Requested dimension is not possible!!!" << std::endl;
        exit(-2);
    }
    return samples;
}

template std::vector<double> pointSampler2dd::nrooks_samples(int N, double *bbox, int dim, bool shuffle);

