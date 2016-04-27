#include "sampler-points.h"

std::vector<double> homogenize_pointsamples2d(std::vector<double> &vec, double* domain){

    int nCoordinates = vec.size();
    std::vector<double> shifted_points(nCoordinates,0.0);
    int nPoints = nCoordinates * 0.5;

    double random_shift_vector[] = {(2*drand48()-1)*domain[2], (2*drand48()-1)*domain[3]};

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

std::vector<double> homogenize_pointsamples(std::vector<double> &vec, double* domain, int nDims){

    int nCoordinates = vec.size();
    std::vector<double> shifted_points(nCoordinates,0.0);
    int nPoints = nCoordinates / double(nDims);

    double random_shift_vector[nDims];
    for(int i=0; i < nDims;i++){
        random_shift_vector[i] = {(2*drand48()-1)*domain[nDims+i]};
    }

    for(int i=0; i < nPoints; i++){

        for(int k = 0; k < nDims; k++){
            shifted_points[nDims*i + k] = vec[nDims*i + k] + random_shift_vector[k];

            if(shifted_points[nDims*i + k] < domain[k])
                shifted_points[nDims*i + k] = domain[nDims+k] + shifted_points[nDims*i + k] - domain[k];
            else if(shifted_points[nDims*i + k] > domain[nDims+k])
                shifted_points[nDims*i + k] = domain[k] + shifted_points[nDims*i + k] - domain[nDims+k];
        }
        //std::cout << "2: " << shifted_points[2*i+0] <<" " << shifted_points[2*i+1] << std::endl;
    }
    return shifted_points;
}

