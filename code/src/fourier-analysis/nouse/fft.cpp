#include <fstream>
#include <complex>
#include <stdlib.h>
#include <iostream>

//#include "./../core/mymacros.h"
#define For(i,N) for(int i=0;i<N;i++)

//#include "fft.h"

//helper function
template <typename T>
void swapQuadrants(T *img, int width, int height)
{
    int halfx = floor(width / 2.f);   // width is no. of cols
    int halfy = floor(height / 2.f);  // height is no. of rows

    For(i, halfy){
        For(j, halfx){
            T temp0 = img[i*width+j];
            T temp1 = img[(i+halfy)*width + (j+halfx)];
            img[i*width+j] = temp1;
            img[(i+halfy)*width + (j+halfx)] = temp0;

            T temp2 = img[i*width + (j+halfx)];
            T temp3 = img[(i+halfy)*width + j];
            img[i*width + (j+halfx)] = temp3;
            img[(i+halfy)*width+j] = temp2;
        }
    }
}

template void swapQuadrants(double *img, int width, int height);
template void swapQuadrants(float *img, int width, int height);

