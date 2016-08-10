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
// 				White Disk within a black pixel
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
// Evaluation returns 1 if p is inside the disk and zero otherwise
/////////////////////////////////////////////////////////////

double PBRTIntegrand::operator () (const Point2D& p) const
{
    
    char command [999];
    double pixelValue(0);
    std::stringstream ss;

    //ss << _pathexec << "/build/pbrt" << " " << _pathscene << "/killeroo-moving/anim-killeroos.pbrt --nxsamples %d --nysamples %d";
    
    ss << _pathexec << "/build/pbrt" << " " << _pathscene << "/killeroo-moving/anim-killeroos.pbrt --xpixel %d --ypixel %d --out %d";
    
    
    //sprintf (command, ss.str().c_str(), _xpixel, _ypixel);
    sprintf (command, ss.str().c_str(), _xpixel, _ypixel, pixelValue);
    
    std::system(command);
    
    return pixelValue ;
}

PBRTIntegrand::~PBRTIntegrand()
{}


