#include "sampler-points.h"

template <typename T>
std::vector<T> homogenize_pointsamples2d(std::vector<T> &vec, T* domain){

    int nCoordinates = vec.size();
    std::vector<T> shifted_points(nCoordinates,0.0);
    int nPoints = nCoordinates * 0.5;

    double random_shift_vector[] = {T(2*drand48()-1)*domain[2], T(2*drand48()-1)*domain[3]};

    for(int i=0; i < nPoints; i++){
        //std::cout << "1: " << vec[2*i+0] <<" " << vec[2*i+1] << std::endl;
        shifted_points[2*i+0] = vec[2*i+0] + random_shift_vector[0];
        shifted_points[2*i+1] = vec[2*i+1] + random_shift_vector[1];

        if(shifted_points[2*i+0] < domain[0])
            shifted_points[2*i+0] = domain[2] + shifted_points[2*i+0] - domain[0];
        else if(shifted_points[2*i+0] > domain[2])
            shifted_points[2*i+0] = domain[0] + shifted_points[2*i+0] - domain[2];

        if(shifted_points[2*i+1] < domain[1])
            shifted_points[2*i+1] = domain[3] + shifted_points[2*i+1] - domain[1];
        else if(shifted_points[2*i+1] > domain[3])
            shifted_points[2*i+1] = domain[1] + shifted_points[2*i+1] - domain[3];

        //std::cout << "2: " << shifted_points[2*i+0] <<" " << shifted_points[2*i+1] << std::endl;

    }
    return shifted_points;
}

template std::vector<double> homogenize_pointsamples2d(std::vector<double> &vec, double* domain);
template std::vector<float> homogenize_pointsamples2d(std::vector<float> &vec, float* domain);

