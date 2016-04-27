#ifndef WRITEPNG_H
#define WRITEPNG_H

#include <iomanip>
#include <FreeImage.h>

#include "./../core/mymacros.h"

template <typename T>
void write_png_rgb( std::string output_file, T* data_ptr, int xRes, int yRes, bool colorOffset=false);

template <typename T>
void write_png_grey( std::string output_file, const T* data_ptr, int xRes, int yRes, bool colorOffset=false);

template <typename T>
void mygamma_correction(T &data);
//void postProcess(const std::string& output_file, float* data_ptr, bool gamma=false, bool org=true, int xStart=0, int xEnd=WIDTH, int yStart=0, int yEnd=HEIGHT, int xRes, int yRes);
//void debug_postProcess(std::string& output_file, float* data_ptr, int xStart=0, int xEnd=WIDTH, int yStart=0, int yEnd=HEIGHT, int xRes, int yRes);

#endif
