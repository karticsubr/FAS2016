#include "write-exr.h"

#ifdef __APPLE__

//nothing else

void writeMultiChannelEXR(std::string name, float *pixels, int width, int height, int nchannels){
    Header header (width, height);
    
    For(k, nchannels){
        //std::stringstream ss;
        //ss << "channel_"<< k;
        //header.channels().insert (ss.str().c_str(), Channel (FLOAT));
        char ch_name[10];
		sprintf(ch_name,"Bin_%04d",k);
		header.channels().insert (ch_name, Channel (FLOAT));
    }
    
    OutputFile file(name.c_str(), header);
    
    FrameBuffer frameBuffer;
    For(k, nchannels){
        //std::stringstream ss;
        //ss << "bin_"<< k;
		char ch_name[10];
		sprintf(ch_name,"Bin_%04d",k);
        frameBuffer.insert (ch_name,
                            Slice (FLOAT,
                                   (char*) &pixels[k*width*height],
                                   sizeof (*pixels) * 1,
                                   sizeof (*pixels) * width));
    }
    
    file.setFrameBuffer (frameBuffer);
    file.writePixels (height);
}


#elif __linux__


using namespace Imf;
using namespace Imath;


void write_exr_rgb(std::string name, const float *pixels, int xRes, int yRes) {
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


void write_exr_rgba(std::string name, const float *pixels, int xRes, int yRes) {
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

/* Writes Luminance (Y value from YUV) image: that is a GrayScale Image */
void write_exr_grey(std::string name, const float *pixels, int xRes, int yRes) {
    Rgba *hrgba = new Rgba[xRes * yRes];

    //Below loop works without any vertical flip of original data (image)
    //We need to flip y-coordinate if we are reading data from PNG file
    // Because in PNG file has origin at Lower-Left Corner.
    //IN EXR image origin is at Upper-Left Corner
	For(r, yRes)
		For(c, xRes)
            hrgba[r*xRes+c] = Rgba(pixels[r*xRes+c], pixels[r*xRes+c],pixels[r*xRes+c],0);
			//hrgba[r*xRes+c] = Rgba(pixels[(yRes-1-r)*xRes+c], pixels[(yRes-1-r)*xRes+c],pixels[(yRes-1-r)*xRes+c],0);

    Box2i displayWindow(V2i(0,0), V2i(xRes-1, yRes-1));
    Box2i dataWindow = displayWindow;

	if(name.compare(name.size()-4, 4,".exr") != 0){
		name.erase(name.end()-4, name.end());
		name += ".exr";
	}

    RgbaOutputFile file(name.c_str(), displayWindow, dataWindow, WRITE_Y);
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

void writeMultiChannelEXR(std::string name, float *pixels, int width, int height, int nchannels){
    Header header (width, height);

    For(k, nchannels){
        //std::stringstream ss;
        //ss << "channel_"<< k;
        //header.channels().insert (ss.str().c_str(), Channel (FLOAT));
        char ch_name[10];
		sprintf(ch_name,"Bin_%04d",k);
		header.channels().insert (ch_name, Channel (FLOAT));
    }

    OutputFile file(name.c_str(), header);

    FrameBuffer frameBuffer;
    For(k, nchannels){
        //std::stringstream ss;
        //ss << "bin_"<< k;
		char ch_name[10];
		sprintf(ch_name,"Bin_%04d",k);
        frameBuffer.insert (ch_name,
                            Slice (FLOAT,
                                   (char*) &pixels[k*width*height],
                                   sizeof (*pixels) * 1,
                                   sizeof (*pixels) * width));
    }

    file.setFrameBuffer (frameBuffer);
    file.writePixels (height);
}

void write_exr_float(std::string name, const float *pixels, int xRes, int yRes){
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

#endif
