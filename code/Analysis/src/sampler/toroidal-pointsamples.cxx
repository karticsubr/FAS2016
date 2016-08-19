#include "sampler-points.h"

std::vector<Point2d> PointSampler2dd::toroidal_samples(std::vector<Point2d> &inSamples, double *domain){
//std::vector<double> homogenize_pointsamples(std::vector<double> &vec, double* domain, int nDims){

    int nPoints = inSamples.size();

    for(int i=0; i < nPoints; i++){

            if(inSamples[i].x() < domain[0])
                inSamples[i].x() = domain[2] + inSamples[i].x() - domain[0];
            else if(inSamples[i].x() > domain[2]){
                //std::cerr << i << " " << inSamples[i].x() << std::endl;
                inSamples[i].x() = domain[0] + inSamples[i].x() - domain[2];
                //std::cerr << i << " " << inSamples[i].x() << std::endl;
            }

            if(inSamples[i].y() < domain[1])
                inSamples[i].y() = domain[3] + inSamples[i].y() - domain[1];
            else if(inSamples[i].y() > domain[3])
                inSamples[i].y() = domain[1] + inSamples[i].y() - domain[3];
    }
    return inSamples;
}

