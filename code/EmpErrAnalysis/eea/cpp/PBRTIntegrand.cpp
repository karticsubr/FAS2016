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

const string PBRTIntegrand::PixelStr = "--pixel" ;
const string PBRTIntegrand::PbrtExePathStr = "--epath" ;
const string PBRTIntegrand::PbrtScenePathStr = "--spath" ;

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
    
    _pathexec = CLParser::FindArgument<std::string>(IntegParams, PbrtExePathStr) ;
    _pathscene = CLParser::FindArgument<std::string>(IntegParams, PbrtScenePathStr) ;
    
    std::vector<double> MultiArgs;
    CLParser::FindMultiArgs<double>(2, MultiArgs, IntegParams, PixelStr) ;
    
    _xpixel = MultiArgs[0];
    _ypixel = MultiArgs[1];
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
// --out : pixel radiance value returned from your PBRT code
/////////////////////////////////////////////////////////////

double PBRTIntegrand::operator () (const Point2D& p) const
{
    
    char command [999];
    double pixelValue(0);
    std::stringstream ss;

    //ss << "/user-home/pbrt-v3/build/pbrt" << " " << "/user-home/pbrt-v3-scene/killeroo-moving/anim-killeroos.pbrt" << " --xpixel %d --ypixel %d --out %f";
    
    ss << _pathexec << " " << _pathscene << " --xpixel %d --ypixel %d --out %f";
    
    sprintf (command, ss.str().c_str(), _xpixel, _ypixel, pixelValue);
    
    std::system(command);
    
    return pixelValue ;
}

PBRTIntegrand::~PBRTIntegrand()
{}


