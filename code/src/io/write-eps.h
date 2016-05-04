#ifndef WRITEEPS_H
#define WRITEEPS_H

#include <iostream>
#include <vector>
void write_eps(std::string filename, const std::vector<double>& points, int nDims=2,
               double radius=2.0, double scale=512.0);

#endif // WRITEEPS_H
