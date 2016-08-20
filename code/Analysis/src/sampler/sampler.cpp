#include "sampler.h"
#include "./../core/cmdlnparser.h"

#include <stdexcept>
#include <unistd.h>
#include <sstream>
#include <random>
#include <iostream>

using std::cout ;
using std::stringstream ;
using std::endl ;
using std::invalid_argument ;

map<string, Sampler*> SamplerPrototype::exemplars ;
SamplerPrototype s1;


/////////////////////////////////////////////
// When implementing a new type of sampler,
//  say MyNewSampler
// add an extra line to "MODIFY THIS" block
/////////////////////////////////////////////
SamplerPrototype::SamplerPrototype()
{
    vector<Sampler*> vs ;
    vs.push_back(new randomSampler());
    vs.push_back(new jitteredSampler());
//    vs.push_back(new gjSampler());
//    vs.push_back(new bjSampler());
    // vs.push_back(new MyNewSampler()); // add a line like this

    for (int i(0); i<vs.size(); i++)
    exemplars[vs[i]->GetType()] = vs[i] ;
}

/////////////////////////////////////////////
// You should not need to modify this
/////////////////////////////////////////////
Sampler* SamplerPrototype::Generate(const vector<string>& SamplerString)
{
    string type = CLParser::FindArgument<string>(SamplerString, CLArg::SamplerType) ;

    map<string, Sampler*>::iterator it = exemplars.find(type) ;
    if (it==exemplars.end()) throw invalid_argument("Unknown sampler type") ;
    return it->second->GenSampler(SamplerString) ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
ostream& operator << (ostream& os, Sampler& s)
{
    for (int i(0); i< s.p.size(); i++)
    os << s.p[i].x << " " << s.p[i].y << endl ;
}


vector<Point2d>& Sampler::Sample(int n)
{
    MTSample(p, n) ;
    return p;
}

Sampler::~Sampler()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Toroidal wrapping of samples
////////////////////////////////////////////////////////////////////////////////////////////////////////////


std::vector<Point2d> Sampler::toroidalWrapping(std::vector<Point2d> &inSamples){
//std::vector<double> homogenize_pointsamples(std::vector<double> &vec, double* domain, int nDims){

    int nPoints = inSamples.size();
    double domain[] = {bBoxMin, bBoxMin, bBoxMax, bBoxMax};

    for(int i=0; i < nPoints; i++){

            if(inSamples[i].x < domain[0])
                inSamples[i].x = domain[2] + inSamples[i].x - domain[0];
            else if(inSamples[i].x > domain[2]){
                //std::cerr << i << " " << inSamples[i].x << std::endl;
                inSamples[i].x = domain[0] + inSamples[i].x - domain[2];
                //std::cerr << i << " " << inSamples[i].x << std::endl;
            }

            if(inSamples[i].y < domain[1])
                inSamples[i].y = domain[3] + inSamples[i].y - domain[1];
            else if(inSamples[i].y > domain[3])
                inSamples[i].y = domain[1] + inSamples[i].y - domain[3];
    }
    return inSamples;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Random
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* randomSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new randomSampler(SamplerParams) ;
}

randomSampler::randomSampler(const vector<string>& SamplerParams)
{
    SamplingType = "random" ;
}

void randomSampler::MTSample(vector<Point2d>& pts, int n) const
{
    std::cerr << "hellow 1"  << std::endl;
    pts.resize(n) ;
    std::cerr << "hellow "  << std::endl;
    std::cerr << pts.size() << std::endl;
    double maxRange = bBoxMax - bBoxMin;
    for (int i=0; i<n; i++)
    {
        double x = drand48();
        double y = drand48();
        ///
        /// Change the range of (x,y) from [0,1) to [bBoxMin, bBoxMax)
        ///
        x = (maxRange * x) + bBoxMin;
        y = (maxRange * y) + bBoxMin;

    pts[i] = Point2d(x,y) ;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Stratified/Jittered sampler (with one sample per strata)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* jitteredSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new jitteredSampler(SamplerParams) ;
}

jitteredSampler::jitteredSampler(const vector<string>& SamplerParams)
{
    SamplingType = "stratified" ;
}

void jitteredSampler::MTSample(vector<Point2d> &pts, int n) const
{
    int sqrtN (floor(sqrt(n))) ;
    double dX(1.0f/(sqrtN)), dY(dX);

    pts.resize(n) ;

    double maxRange = bBoxMax - bBoxMin;

    for (int r=0; r<sqrtN; r++){
        for (int c=0; c<sqrtN; c++){
            double x = (c + drand48()) * dX;
            double y = (r + drand48()) * dY;

            ///
            /// Change the range of (x,y) from [0,1) to [bBoxMin, bBoxMax)
            ///
            x = (maxRange * x) + bBoxMin;
            y = (maxRange * y) + bBoxMin;

            pts[r*sqrtN+c] = Point2d(x, y);
        }
    }
}

