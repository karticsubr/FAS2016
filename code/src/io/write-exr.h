#ifndef WRITE_EXR_H
#define WRITE_EXR_H
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ImfInputFile.h>
#include <ImfOutputFile.h>
#include <ImfChannelList.h>
#include <ImfFrameBuffer.h>
#include <ImfRgbaFile.h>
#include <half.h>
#include <assert.h>

#include "./../core/mymacros.h"

#ifdef __linux__

void write_exr_grey(std::string name, const float *pixels, int xRes, int yRes);
void write_exr_rgb(std::string name, const float *pixels, int xRes, int yRes);
void write_exr_rgba(std::string name, const float *pixels, int xRes, int yRes);
void write_exr_float(std::string name, const float *pixels, int xRes, int yRes);
void writeMultiChannelEXR(std::string name, float* pixels, int width, int height, int nchannels);

#elif __APPLE__

using namespace Imf;
using namespace Imath;


template <typename T>
void write_exr_grey(std::string name, T pixels, int xRes, int yRes){
    Rgba *hrgba = new Rgba[xRes * yRes];

    //Below loop works without any vertical flip of original data (image)
    //We need to flip y-coordinate if we are reading data from PNG file
    // Because in PNG file has origin at Lower-Left Corner.
    //IN EXR image origin is at Upper-Left Corner
    For(r, yRes)
    For(c, xRes)
    hrgba[r*xRes+c] = Rgba(pixels[(r)*xRes+c], pixels[(r)*xRes+c],pixels[(r)*xRes+c], 1);
    //hrgba[r*xRes+c] = Rgba(pixels[(yRes-1-r)*xRes+c], pixels[(yRes-1-r)*xRes+c],pixels[(yRes-1-r)*xRes+c],0);

    Box2i displayWindow(V2i(0,0), V2i(xRes-1, yRes-1));
    Box2i dataWindow = displayWindow;

    if(name.compare(name.size()-4, 4,".exr") != 0){
        name.erase(name.end()-4, name.end());
        name += ".exr";
    }

    RgbaOutputFile file(name.c_str(), displayWindow, dataWindow, WRITE_RGBA);
    file.setFrameBuffer(hrgba, 1, xRes);
    try {
        file.writePixels(yRes);
    }
    catch (const std::exception &e) {
        fprintf(stderr, "Unable to write image file \"%s\": %s", name.c_str(),
                e.what());
    }

    delete[] hrgba;

}

template <typename T>
void write_exr_rgb(std::string name, T pixels, int xRes, int yRes){
    Rgba *hrgba = new Rgba[xRes * yRes];
    //Problem: causes vertical flip of the data (image)
    //for (int i = 0; i < xRes * yRes; ++i)
    //hrgba[i] = Rgba(pixels[4*i], pixels[4*i+1], pixels[4*i+2], 1.);

    For(r, yRes)
    For(c, xRes)
    hrgba[r*xRes+c] = Rgba(pixels[3*(r*xRes+c)+0], pixels[3*(r*xRes+c)+1], pixels[3*(r*xRes+c)+2], 1);

    Box2i displayWindow(V2i(0,0), V2i(xRes-1, yRes-1));
    Box2i dataWindow = displayWindow;

    if(name.compare(name.size()-4, 4,".exr") != 0){
        name.erase(name.end()-4, name.end());
        name += ".exr";
    }

    RgbaOutputFile file(name.c_str(), displayWindow, dataWindow, WRITE_RGBA);
    file.setFrameBuffer(hrgba, 1, xRes);
    try {
        file.writePixels(yRes);
    }
    catch (const std::exception &e) {
        fprintf(stderr, "Unable to write image file \"%s\": %s", name.c_str(),
                e.what());
    }

    delete[] hrgba;
}

template <typename T>
void write_exr_rgba(std::string name, T pixels, int xRes, int yRes){
    Rgba *hrgba = new Rgba[xRes * yRes];
    //Problem: causes vertical flip of the data (image)
    //for (int i = 0; i < xRes * yRes; ++i)
    //hrgba[i] = Rgba(pixels[4*i], pixels[4*i+1], pixels[4*i+2], 1.);

    For(r, yRes)
    For(c, xRes)
    hrgba[r*xRes+c] = Rgba(pixels[4*(r*xRes+c)+0], pixels[4*(r*xRes+c)+1], pixels[4*(r*xRes+c)+2], pixels[4*(r*xRes+c)+3]);

    Box2i displayWindow(V2i(0,0), V2i(xRes-1, yRes-1));
    Box2i dataWindow = displayWindow;

    if(name.compare(name.size()-4, 4,".exr") != 0){
        name.erase(name.end()-4, name.end());
        name += ".exr";
    }

    RgbaOutputFile file(name.c_str(), displayWindow, dataWindow, WRITE_RGBA);
    file.setFrameBuffer(hrgba, 1, xRes);
    try {
        file.writePixels(yRes);
    }
    catch (const std::exception &e) {
        fprintf(stderr, "Unable to write image file \"%s\": %s", name.c_str(),
                e.what());
    }

    delete[] hrgba;
}

template <typename T>
void write_exr_float(std::string name, T pixels, int xRes, int yRes){
    Header header (xRes, yRes);
    header.channels().insert ("Z", Channel (FLOAT));

    if(name.compare(name.size()-4, 4,".exr") != 0){
        name.erase(name.end()-4, name.end());
        name += ".exr";
    }

    OutputFile file(name.c_str(), header);

    FrameBuffer frameBuffer;

    //frameBuffer.insert ("G", Slice (HALF, (char *) pixels, sizeof (*pixels) * 1, sizeof (*pixels) * xRes));
    frameBuffer.insert ("Z", Slice (FLOAT, (char *) pixels, sizeof (*pixels) * 1, sizeof (*pixels) * xRes));

    file.setFrameBuffer (frameBuffer);
    file.writePixels (yRes);
}

void writeMultiChannelEXR(std::string name, float* pixels, int width, int height, int nchannels);

#endif

#endif
