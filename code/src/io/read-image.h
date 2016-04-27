#ifndef READIMAGE_H
#define READIMAGE_H

#include <iostream>
#include <FreeImage.h>

void loadImage(std::string name, float** pixels, int& width, int& height, std::string type="", bool rowmajor=true);
void loadGreyImage(std::string name, float** pixels, int& width, int& height,	FIBITMAP *bmp, bool rowmajor=true);
void loadRGBImage(std::string name, float** pixels	,int& width, int& height, 	FIBITMAP *bmp, bool rowmajor=true);
void loadRGBtoYImage(std::string name, float** pixels	,int& width, int& height,	FIBITMAP *bmp, bool rowmajor=true);
void loadRGBtoYUVImage(std::string name, float** pixels	,int& width, int& height,	FIBITMAP *bmp, bool rowmajor=true);
bool ReadEXR(std::string name, float *&rgba, int &xRes, int &yRes, bool &hasAlpha);
float *readMultiChannelEXR(std::string fileName, int *width, int *height, int *nbins);
#endif
