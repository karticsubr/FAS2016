#include "sampler-points.h"
#include <math.h>

template<int Dimension, typename T>
std::vector<T> PointSampler<Dimension, T>::multijitter_samples(int N, T* bBox, int dim, bool shuffle){

    std::vector<T> samples;
    //mjpoint* samples = new mjpoint[N]();

    double maxRange = bBox[2*dim-1] - bBox[dim-1];

    float sqN = sqrt(N);
    int iN = sqN;
    float fracN = sqN-iN;
    if( fracN != 0){
        std::cerr << "Only N with integer square root is accepted !!!" << std::endl;
        exit(-1);
    }
    int dimension = sqN;
    int ncolStrata = dimension;
    int nrowStrata = dimension;

    for(int r = 0; r < dimension; r++){
        for(int c = 0; c < dimension; c++){
            T sx = drand48();
            T sy = drand48();
            T x = (c + ((r + sx) / nrowStrata))/ncolStrata;
            T y = (r + ((c + sy) / ncolStrata))/nrowStrata;

            T subindx = maxRange * x - bBox[dim-1];
            T subindy = maxRange * y - bBox[dim-1];

//            int index = r * ncolStrata + c;

            samples.push_back(subindx);
            samples.push_back(subindy);
        }
    }

    if(shuffle){

        for(int j = 0; j < ncolStrata; j++){
            for(int i = 0; i < nrowStrata; i++){
                int k = j + drand48() * (ncolStrata - j);

                int index = j * nrowStrata + i;
                int indexk = k * nrowStrata + i;
                std::swap(samples[2*index + 0], samples[2*indexk + 0]);
            }
        }

        for(int i = 0; i < ncolStrata; i++){
            for(int j = 0; j < nrowStrata; j++){
                int k = i + drand48() * (ncolStrata - i);

                int index = j * nrowStrata + i;
                int indexk = j * nrowStrata + k;

                std::swap(samples[2*index + 1], samples[2*indexk + 1]);
            }
        }
    }
    return samples;
}

template std::vector<double> pointSampler2dd::multijitter_samples(int N, double *bbox, int dim, bool shuffle);

