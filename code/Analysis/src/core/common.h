//
//  common.h
//  SamplingAnalysis
//
//  Created by Gurpreet Singh Bagga on 04/08/16.
//
//

#ifndef SamplingAnalysis_common_h
#define SamplingAnalysis_common_h

#include <iostream>

#define PI 3.14159265358979323846264338
#define deg2rad (PI / 180.0)
#define rad2deg (180.0 / PI)
#define twopi 2.0*PI
#define pibytwo PI*0.5

inline void paddedzerosN(std::string &s1, int fileNumber){

    if(fileNumber > 100000){
        std::cerr << "fileNumber is too big !!!" << std::endl;
        exit(-2);
    }

    do{
        s1 = "0"+s1;
    }while(s1.length() != 6);
}

class IO{
public:
    static void WriteEXRrgba(const std::string& filename, const float* rgb, int width, int height);
    static void WriteEXRrgb(const std::string& filename, const float* rgb, int width, int height);
    static void WriteEXRgrey(const std::string &filename, const float *rgb, int width, int height);
    static bool LoadEXRrgba(const char* filename, float** rgba, int *w, int *h);
};


#endif

