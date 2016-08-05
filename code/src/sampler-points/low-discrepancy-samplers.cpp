#include "sampler-points.h"
#include "./../core/utils.h"

template<int Dimension, typename T>
std::vector<T> PointSampler<Dimension, T>::halton_sequence_samples(const int &N, T* bBox, int dim, bool scramble){
    std::vector<T> results;

    T maxRange = bBox[2*dim-1] - bBox[dim-1];

    for(int i=0; i<N; i++){
        T x = radicalInverse(i,2);
        T y = radicalInverse(i,3);

        results.push_back(maxRange * x - bBox[dim-1]);
        results.push_back(maxRange * y - bBox[dim-1]);
    }
    return results;
}

template<int Dimension, typename T>
std::vector<T> PointSampler<Dimension, T>::hammersley_sequence_samples(const int &N, T* bBox,int dim, bool scramble){
    std::vector<T> results;

    T maxRange = bBox[2*dim-1] - bBox[dim-1];

    for(int i=0; i<N; i++){

        T x = i / T(N);
        T y = radicalInverse(i,2);

        results.push_back(maxRange * x - bBox[dim-1]);
        results.push_back(maxRange * y - bBox[dim-1]);
    }
    return results;
}


unsigned int fiboTable[32] =
{ 1,1,2,3,5,8,13,21,34,55,89,144,233,377,610,987,
1597,2584,4181,6765,10946,17711,28657,46368,75025,
121393,196418,317811,514229,832040,1346269,2178309 };

std::vector<double> fibonacci_lattice_samples(int dim1, bool scramble){

    std::vector<double> samples;

    double scrambler[] = {drand48(), drand48()};

    int N = fiboTable[dim1];

    for(int i=0;i<N;i++){
        double x = (i/float(N));
        double y = ((i * fiboTable[dim1-1]) % fiboTable[dim1]) / (double) fiboTable[dim1];

        if(scramble){
            x += scrambler[0];
            y += scrambler[1];
            if(x > 1.0)
                x = x - 1.0;
            if(y > 1.0)
                y = y - 1.0;
        }

        samples.push_back(x);
        samples.push_back(y);
    }
    return samples;
}

inline float VanDerCorput(uint32_t index, uint32_t scramble);
inline float Sobol(uint32_t index, uint32_t scramble);

std::vector<double> vandercorput_sequence_samples(const int &N){

    uint32_t tempN = N;

    std::vector<double> samples;
    uint32_t scramble[] = {uint32_t(lrand48()), uint32_t(lrand48())};

    for(uint32_t index = 0; index < tempN; index++){
        float fx = VanDerCorput(index, 0);//scramble[0]);
        float fy = Sobol(index, 0);//scramble[1]);
        samples.push_back(fx);
        samples.push_back(fy);
    }
    return samples;
}

inline float VanDerCorput(uint32_t index, uint32_t scramble){
    index = (index << 16 ) | (index >> 16);
    index = ((index & 0x00ff00ff) << 8) | ((index & 0xff00ff00) >> 8);
    index = ((index & 0x0f0f0f0f) << 4) | ((index & 0x0f0f0f0f) >> 4);
    index = ((index & 0x33333333) << 2) | ((index & 0x33333333) >> 2);
    index = ((index & 0x55555555) << 1) | ((index & 0x55555555) >> 1);
    index ^= scramble;
    return ((index >> 8) & 0xffffff) / float(1 << 24);
}

inline float Sobol(uint32_t index, uint32_t scramble){
    for(uint32_t v = 1 << 31; index != 0; index >>=1, v ^= v >> 1)
        if (index & 0x1) scramble ^= v;
    return ((scramble>>8) & 0xffffff) / float (1 << 24);
}

template std::vector<double> pointSampler2dd::halton_sequence_samples(const int& N, double *bbox, int dim, bool scramble);
template std::vector<double> pointSampler2dd::hammersley_sequence_samples(const int& N, double *bbox, int dim, bool scramble);

