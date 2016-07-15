#include "write-eps.h"

#include <fstream>

// export sites to an EPS image
void write_eps(std::string filename, const std::vector<double> &points, int nDims, double radius, double scale) {

    if(filename.compare(filename.size()-4, 4,".eps") != 0){
        filename.erase(filename.end()-4, filename.end());
        filename += ".eps";
    }


    std::ofstream os;
    os.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);

    radius /= scale;

    os << "%!PS-Adobe-3.1 EPSF-3.0\n";
    os << "%%HiResBoundingBox: " << -radius << " " << -radius << " " << scale+radius << " " << scale+radius << "\n";
    os << "%%BoundingBox: " << -radius << " " << -radius << " " << scale+radius << " " << scale+radius << "\n";
    os << "%%CropBox: " << -radius << " " << -radius << " " << scale+radius << " " << scale+radius << "\n";
    os << "/radius { " << radius << " } def\n";
    os << "/p { radius 0 360 arc closepath fill stroke } def\n";
    os << "gsave " << scale << " " << scale << " scale\n";

    os << "0 0 0 setrgbcolor\n";

    if(nDims == 1){
        int npoints = points.size();
        for (unsigned int i = 0; i < npoints; ++i) {
            os << points[i] << " " << 0.5 << " p\n";
        }
    }
    else{
        int npoints = points.size() * 0.5;
        for (unsigned int i = 0; i < npoints; ++i) {
            os << points[2*i] << " " << points[2*i+1] << " p\n";
        }
    }
    os << "grestore\n";
    os.close();

    //  return true;
}
