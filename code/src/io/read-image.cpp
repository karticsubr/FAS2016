#include <stdio.h>
#include <stdlib.h>
#include <ImfInputFile.h>
#include <ImfOutputFile.h>
#include <ImfChannelList.h>
#include <ImfFrameBuffer.h>
#include <ImfRgbaFile.h>
#include <half.h>
#include <assert.h>

using namespace Imf;
using namespace Imath;

#include "read-image.h"
#include "./../core/mymacros.h"

//Adopted from PBRT2-src code
bool ReadEXR(std::string name, float *&rgba, int &xRes, int &yRes, bool &hasAlpha){
    try {
        InputFile file(name.c_str());
        Box2i dw = file.header().dataWindow();
        xRes = dw.max.x - dw.min.x + 1;
        yRes = dw.max.y - dw.min.y + 1;

        half *hrgba = new half[4 * xRes * yRes];

        // for now...
        hasAlpha = true;
        int nChannels = 4;

        hrgba -= 4 * (dw.min.x + dw.min.y * xRes);
        FrameBuffer frameBuffer;
        frameBuffer.insert("R", Slice(HALF, (char *)hrgba,
                                      4*sizeof(half), xRes * 4 * sizeof(half), 1, 1, 0.0));
        frameBuffer.insert("G", Slice(HALF, (char *)hrgba+sizeof(half),
                                      4*sizeof(half), xRes * 4 * sizeof(half), 1, 1, 0.0));
        frameBuffer.insert("B", Slice(HALF, (char *)hrgba+2*sizeof(half),
                                      4*sizeof(half), xRes * 4 * sizeof(half), 1, 1, 0.0));
        frameBuffer.insert("A", Slice(HALF, (char *)hrgba+3*sizeof(half),
                                      4*sizeof(half), xRes * 4 * sizeof(half), 1, 1, 1.0));

        file.setFrameBuffer(frameBuffer);
        file.readPixels(dw.min.y, dw.max.y);

        hrgba += 4 * (dw.min.x + dw.min.y * xRes);
        rgba = new float[nChannels * xRes * yRes];
        for (int i = 0; i < nChannels * xRes * yRes; ++i)
        rgba[i] = hrgba[i];
        delete[] hrgba;
    } catch (const std::exception &e) {
        fprintf(stderr, "Unable to read image file \"%s\": %s", name.c_str(), e.what());
        return NULL;
    }

    return rgba;
}

float *readMultiChannelEXR(std::string fileName, int *width, int *height, int *nbins){

    float *data;

    try {
        InputFile file (fileName.c_str());

        Box2i dw = file.header().dataWindow();

        *width = dw.max.x - dw.min.x + 1;
        *height = dw.max.y - dw.min.y + 1;
        int nhnc = (*width) * (*height);

        const ChannelList &channelList = file.header().channels();

        //zPixels.resizeErase (height, width);

        FrameBuffer frameBuffer;

        char ch_name[10];
        int nbin =0;
        sprintf(ch_name,"Bin_%04d",nbin);
        //printf("%s\n",ch_name);
        const Channel *channelPtr = channelList.findChannel(ch_name);


        while(channelPtr)
        {
            nbin++;
            sprintf(ch_name,"Bin_%04d",nbin);
            // printf("%s\n",ch_name);
            channelPtr = channelList.findChannel(ch_name);
        }


        //malloc for the whole array
        data = (float*) malloc(nhnc*nbin*sizeof(float));


        for(int i=0;i<nbin;i++)
        {
            sprintf(ch_name,"Bin_%04d",i);
            // printf("%s\n",ch_name);

            frameBuffer.insert(ch_name,
                               Slice(FLOAT,
                                     (char*)(&data[i*nhnc]),
                                     sizeof(float),
                                     (*width) * sizeof(float)));

        }


        file.setFrameBuffer (frameBuffer);

        try {
            file.readPixels (dw.min.y, dw.max.y);

        }
        catch (const std::exception &e) {
            data = NULL;
        }


        *nbins = nbin;
        return data;

    }
    catch (const std::exception &e) {
        printf("Error reading file: %s\n",fileName.c_str());
        exit(-1);
    }

}

void loadGreyImage(std::string name, float** pixels, int& width, int& height,	FIBITMAP *bmp, bool rowmajor){
/*	FreeImage_Initialise ();
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(name.c_str());
    FIBITMAP *bmp = FreeImage_Load(fif, name.c_str());

     width = FreeImage_GetWidth(bmp);
     height = FreeImage_GetHeight(bmp);
    int bpp = FreeImage_GetBPP(bmp);
*/
    *pixels = new float[3*width*height];

    //FIBITMAP* outImgbitmap = FreeImage_Allocate(width, height, bpp);
    BYTE *bits = FreeImage_GetBits(bmp);

    int rows =(int) height;
    int cols =(int) width;
    float rgb[3];

    For(r, rows){
        For(c, cols){
            For(k,3){
                rgb[k] = float(bits[r*cols+c]);
                if(rowmajor)
                    (*pixels)[3*(r*cols+c)+k] = rgb[k];
                else
                    (*pixels)[3*(r+rows*c)+k] = rgb[k];
            }
        }
    }

//	FreeImage_Unload(bmp);
//	FreeImage_DeInitialise (); //Cleanup !
}


void loadRGBImage(std::string name, float** pixels, int& width, int& height,	FIBITMAP *bmp, bool rowmajor){
/*	FreeImage_Initialise ();
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(name.c_str());
    FIBITMAP *bmp = FreeImage_Load(fif, name.c_str());

     width = FreeImage_GetWidth(bmp);
     height = FreeImage_GetHeight(bmp);
    int bpp = FreeImage_GetBPP(bmp);
*/
    *pixels = new float[3*width*height];

    //FIBITMAP* outImgbitmap = FreeImage_Allocate(width, height, bpp);

    RGBQUAD color;

    int rows =(int) height;
    int cols =(int) width;
    float rgb[3];

    For(r, rows){
        For(c, cols){
            FreeImage_GetPixelColor(bmp, c, r, &color);
            rgb[0] = (float)color.rgbRed ;
            rgb[1] = (float)color.rgbGreen;
            rgb[2] = (float)color.rgbBlue ;

            if(rowmajor)
                For(k,3)
                (*pixels)[3*(r*cols+c)+k] = rgb[k];
            else
                For(k,3)
                (*pixels)[3*(r+rows*c)+k] = rgb[k];
        }
    }

//	FreeImage_Unload(bmp);
//	FreeImage_DeInitialise (); //Cleanup !
}


void loadRGBtoYImage(std::string name, float** pixels, int& width, int& height,	FIBITMAP *bmp, bool rowmajor){
/*	FreeImage_Initialise ();
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(name.c_str());
    FIBITMAP *bmp = FreeImage_Load(fif, name.c_str());

     width = FreeImage_GetWidth(bmp);
     height = FreeImage_GetHeight(bmp);
    int bpp = FreeImage_GetBPP(bmp);
*/

    *pixels = new float[width*height];

    //FIBITMAP* outImgbitmap = FreeImage_Allocate(width, height, bpp);

    RGBQUAD color;

    int rows =(int) height;
    int cols =(int) width;
    float rgb[3];
    float yuv[3];

    For(r, rows){
        For(c, cols){
            FreeImage_GetPixelColor(bmp, c, r, &color);
            rgb[0] = (float)color.rgbRed ;
            rgb[1] = (float)color.rgbGreen;
            rgb[2] = (float)color.rgbBlue ;

            yuv[0] = 0.299*rgb[0]		+ 0.587*rgb[1]		+ 0.114*rgb[2];
            yuv[1] = -0.14713*rgb[0]	- 0.28886*rgb[1]	+ 0.436*rgb[2];
            yuv[2] = 0.615*rgb[0]		- 0.51499*rgb[1]	- 0.10001*rgb[2];

            if(rowmajor)
                    (*pixels)[r*cols+c] = yuv[0];
            else
                    (*pixels)[r+rows*c] = yuv[0];
        }
    }
    //std::cerr << "RGB to Y done!" << std::endl;
//	FreeImage_Unload(bmp);
//	FreeImage_DeInitialise (); //Cleanup !
}

void loadRGBtoYUVImage(std::string name, float** pixels, int& width, int& height,	FIBITMAP *bmp, bool rowmajor){
/*	FreeImage_Initialise ();
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(name.c_str());
    FIBITMAP *bmp = FreeImage_Load(fif, name.c_str());

     width = FreeImage_GetWidth(bmp);
     height = FreeImage_GetHeight(bmp);
    int bpp = FreeImage_GetBPP(bmp);
*/

    *pixels = new float[3*width*height];

    //FIBITMAP* outImgbitmap = FreeImage_Allocate(width, height, bpp);

    RGBQUAD color;

    int rows =(int) height;
    int cols =(int) width;
    float rgb[3];
    float yuv[3];

    For(r, rows){
        For(c, cols){
            FreeImage_GetPixelColor(bmp, c, r, &color);
            rgb[0] = (float)color.rgbRed ;
            rgb[1] = (float)color.rgbGreen;
            rgb[2] = (float)color.rgbBlue ;

            yuv[0] = 0.299*rgb[0]		+ 0.587*rgb[1]		+ 0.114*rgb[2];
            yuv[1] = -0.14713*rgb[0]	- 0.28886*rgb[1]	+ 0.436*rgb[2];
            yuv[2] = 0.615*rgb[0]		- 0.51499*rgb[1]	- 0.10001*rgb[2];

            if(rowmajor)
                For(k,3)
                    (*pixels)[3*(r*cols+c)+k] = yuv[k];
            else
                For(k,3)
                    (*pixels)[3*(r+rows*c)+k] = yuv[k];
        }
    }
    std::cerr << "RGB to YUV done!" << std::endl;
//	FreeImage_Unload(bmp);
//	FreeImage_DeInitialise (); //Cleanup !
}

void loadImage(std::string name, float** pixels, int& width, int& height, std::string type, bool rowmajor){
    FreeImage_Initialise ();
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(name.c_str());
    FIBITMAP *bmp = FreeImage_Load(fif, name.c_str());

    width = FreeImage_GetWidth(bmp);
    height = FreeImage_GetHeight(bmp);
    int bpp = FreeImage_GetBPP(bmp);

    std::cerr << "Image info: width: " << width <<" height: "<<  height <<" bpp: " << bpp << std::endl;

    if(bpp == 8)
        loadGreyImage(name, pixels, width, height,bmp, rowmajor);
    else{
        if(type == "yuv" || type == "YUV")
            loadRGBtoYUVImage(name, pixels, width, height,bmp, rowmajor);
        else if(type == "y" || type == "Y" || type == "grey" || type == "gray")
            loadRGBtoYImage(name, pixels, width, height,bmp, rowmajor);
        else if(type == "rgb" || type == "RGB")
            loadRGBImage(name, pixels, width, height,bmp, rowmajor);
        else
            loadRGBImage(name, pixels, width, height,bmp, rowmajor);
    }

    FreeImage_Unload(bmp);
    FreeImage_DeInitialise (); //Cleanup !
}



