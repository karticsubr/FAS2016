#include <Analyzer.h>
#include <PointAnalyzer.h>
#include <FourierAnalyzer.h>
#include <VarianceAnalyzer.h>
#include "./../core/cmdlnparser.h"

#include <iostream>
#include <fstream>

#include <sstream>
#include <sys/stat.h>

using namespace eea;

const string Analyzer::NSampStr = "--n" ;
const string Analyzer::nTrialsStr = "--t" ;

map<string, Analyzer*> AnalyzerPrototype::exemplars ;
AnalyzerPrototype a1;

Analyzer::~Analyzer()
{}

/////////////////////////////////////////////
// When implementing a new type of sampler,
//  say MyNewSampler
// add an extra line to "MODIFY THIS" block
/////////////////////////////////////////////
AnalyzerPrototype::AnalyzerPrototype()
{
    vector<Analyzer*> vs ;
    vs.push_back(new PointAnalyzer());
    vs.push_back(new FourierAnalyzer());
    vs.push_back(new VarianceAnalyzer());
    // vs.push_back(new MyNewSampler()); // add a line like this

    for (int i(0); i<vs.size(); i++)
    exemplars[vs[i]->GetType()] = vs[i] ;
}

Analyzer* AnalyzerPrototype::Generate(Sampler* s, const vector<string>& AnalyzerString, const vector<std::string> &IntegString)
{
    string type = CLParser::FindArgument<string>(AnalyzerString, CLArg::AnalyzerType) ;

    std::cerr << "Analyzer type: " << type << std::endl;

    map<string, Analyzer*>::iterator it = exemplars.find(type) ;
    if (it==exemplars.end()) throw invalid_argument("Unknown analyzer type") ;
    return it->second->createAnalyzer(s, AnalyzerString, IntegString) ;
}


void Analyzer::WriteEPS(string& filename, double radius, double scale) const{

    if(filename.compare(filename.size()-4, 4,".eps") != 0){
        filename.erase(filename.end()-4, filename.end());
        filename += ".eps";
    }

    std::ofstream os;
    os.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);

    radius /= scale;

    os << "%!PS-Adobe-3.1 EPSF-3.0\n";
    os << "%%HiResBoundingBox: " << -radius << " " << -radius << " " << scale+radius << " " << scale+radius << "\n";
    os << "%%BoundingBox: " << -radius << " " << -radius << " " << scale+radius << " " << scale+radius << "\n";
    os << "%%CropBox: " << -radius << " " << -radius << " " << scale+radius << " " << scale+radius << "\n";
    os << "/radius { " << radius << " } def\n";
    os << "/p { radius 0 360 arc closepath fill stroke } def\n";
    os << "gsave " << scale << " " << scale << " scale\n";

    os << "0 0 0 setrgbcolor\n";

    for (unsigned int i = 0; i < _pts.size(); ++i) {
        os << _pts[i].x() << " " << _pts[i].y() << " p\n";
    }
    os << "grestore\n";
    os.close();

}

/*
template <typename T>
void Analyzer::WriteEXRgrey(std::string name, const T *pixels, int xRes, int yRes) const {
    using namespace Imf;
    using namespace Imath;

    Rgba *hrgba = new Rgba[xRes * yRes];

    for(int r = 0; r < yRes; r++)
        for(int c = 0; c < xRes; c++)
            hrgba[r*xRes+c] = Rgba(pixels[(r*xRes+c)], pixels[(r*xRes+c)], pixels[(r*xRes+c)], 1);

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

template void Analyzer::WriteEXRgrey(std::string name, const float *pixels, int xRes, int yRes) const;
*/


