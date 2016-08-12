//
//  PBRTIntegrand.cpp
//  eea
//
//  Created by Gurpreet Singh Bagga on 10/08/16.
//
//

#include <iostream>
#include <PBRTIntegrand.h>
#include <cmdlnparser.h>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h> //to use getcwd(cwd, sizeof(cwd));

#include <ImfRgba.h>
#include <ImfRgbaFile.h>


const string PBRTIntegrand::CropStr = "--crop" ;
const string PBRTIntegrand::PbrtExecPathStr = "--epath" ;
const string PBRTIntegrand::PbrtScenePathStr = "--spath" ;
const string PBRTIntegrand::PythonScriptPathStr = "--pypath" ;
const string PBRTIntegrand::ExrImgNameStr = "--img" ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				PBRT Integrator to render a given pixel
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Integrand* PBRTIntegrand::GenIntegrand(const vector<string>& IntegParams)
{
    return new PBRTIntegrand(IntegParams) ;
}

/////////////////////////////////////////////
// Constructor:
/////////////////////////////////////////////
PBRTIntegrand::PBRTIntegrand(const vector<string>& IntegParams)
{
    IntegrandType = "Pbrt";
    
    _pathexec = CLParser::FindArgument<std::string>(IntegParams, PbrtExecPathStr) ;
    _pathscene = CLParser::FindArgument<std::string>(IntegParams, PbrtScenePathStr) ;
    _pathpyscript = CLParser::FindArgument<std::string>(IntegParams, PythonScriptPathStr) ;
    _imgname = CLParser::FindArgument<std::string>(IntegParams, ExrImgNameStr) ;
    
    std::vector<double> MultiArgs;
    CLParser::FindMultiArgs(4, MultiArgs, IntegParams, CropStr) ;

    for(int i = 0; i < 4; i++)
        _crop[i] = MultiArgs[i];
}

/////////////////////////////////////////////////////////////
// Evaluation returns the pixel radiance computed from the
// PBRT that is on your system.
// User need to modify their PBRT version to render only the
// given input pixel (_xpixel, _ypixel) and
// to make PBRT return a value for that pixel instead
// of an image. Use the following flags:
// --pixel : pixel coordinates to be rendered from the PBRT code
// --epath : path to your PBRT executable on your machine
// --spath : path to your PBRT Scene file (.pbrt)
// --out : pixel radiance value returned from your PBRT code which
// will be stored in the pixelValue variable.

// You can change the format of the PBRT call below according
// to your coding style.

// NOTE: argument Point2D p is not used in the operator call below
// PBRTIntegrand uses the samplers directly from the PBRT.
/////////////////////////////////////////////////////////////

double PBRTIntegrand::operator () (const Point2D& p, const string &SamplerType) const
{
    char pythonCommand [999], cwd[999];
    std::stringstream ss;
    
    ///
    /// For PBRTIntegrand the p argument contains the number of samples information
    ///
    int N = p.x;
    
    ///
    /// Here is your python script to update the crop window size in the pbrt scene file (.pbrt) with
    /// the values provided from the command line.
    ///
    ss << "python " << _pathpyscript << " " << _pathscene << " " << SamplerType << " %d %f %f %f %f";
    std::cerr << ss.str() << std::endl;
    
    ///
    /// Passing arguments to the python script
    /// Provide N x1 x2 y1 y2 from the command line to fill _crop[4]
    ///
    sprintf(pythonCommand, ss.str().c_str(), N, _crop[0], _crop[1], _crop[2], _crop[3]);
    
    ///
    /// Running python script to change the crop window size of the PBRT Scene File
    ///
    std::system(pythonCommand);
    
    ///
    /// reinitialize ss stringstream
    ///
    ss.str(std::string());
    
    ///
    /// prepare stringstream to call the pbrt with the scenefile
    ///
    ss << _pathexec << " " << _pathscene << " --outfile " <<  _imgname;
    
    ///
    /// Call PBRT
    ///
    std::system(ss.str().c_str());
    
    ///
    /// Read the image generated from PBRT
    ///
    getcwd(cwd, sizeof(cwd));
    ss.str(std::string());
    ss << cwd << "/" << _imgname;
    std::cerr << ss.str() << std::endl;
    int width =0, height =0;
    float* pixels = ReadImageEXR(ss.str(), &width, &height);

    ///
    ///To verify that pixels carry the correct image;
    ///
    //WriteImageEXR("test.exr", pixels,  width, height);
    
    ///
    /// Average the image over all the pixels to return the output value
    ///
    double integral = 0.0;
    for(int i=0; i< 3 * width * height; i++){
        integral += pixels[i];
    }

    integral /= float(3.0 * width *height);
    std::cerr << integral << " "<< width <<" " << height << std::endl;
    
    return integral;
}

float* PBRTIntegrand::ReadImageEXR(const std::string &name, int *width, int *height) const{
    using namespace Imf;
    using namespace Imath;
    
    try {
        RgbaInputFile file(name.c_str());
        Box2i dw = file.dataWindow();
        *width = dw.max.x - dw.min.x + 1;
        *height = dw.max.y - dw.min.y + 1;
        std::vector<Rgba> pixels(*width * *height);
        file.setFrameBuffer(&pixels[0] - dw.min.x - dw.min.y * *width, 1,
                            *width);
        file.readPixels(dw.min.y, dw.max.y);
        
        float *ret = new float[3 * *width * *height]();
        //RGBSpectrum *ret = new RGBSpectrum[*width * *height];
        for (int i = 0; i < *width * *height; ++i) {
            float frgb[3] = {pixels[i].r, pixels[i].g, pixels[i].b};
            //ret[i] = RGBSpectrum::FromRGB(frgb);
            for(int k=0;k<3;k++)
                ret[3*i+k] = frgb[k];
        }
        //Info("Read EXR image %s (%d x %d)", name.c_str(), *width, *height);
        return ret;
    } catch (const std::exception &e) {
        printf("Unable to read image file \"%s\": %s", name.c_str(), e.what());
    }
    
    return NULL;
}

void PBRTIntegrand::WriteImageEXR(std::string name, const float *pixels, int xRes, int yRes) const {
    using namespace Imf;
    using namespace Imath;
    
    Rgba *hrgba = new Rgba[xRes * yRes];
    
    for(int r = 0; r < yRes; r++)
        for(int c = 0; c < yRes; c++)
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


PBRTIntegrand::~PBRTIntegrand()
{}


