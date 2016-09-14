#include <sampler.h>
#include <cmdlnparser.h>

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
        vs.push_back(new gridSampler());
        vs.push_back(new jitteredSampler());
        vs.push_back(new gjSampler());
        vs.push_back(new bjSampler());
        vs.push_back(new latinhypercubeSampler());
        vs.push_back(new haltonSampler());
        vs.push_back(new sobolSampler());
        vs.push_back(new zerotwosequenceSampler());
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
    pts.resize(n) ;
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Regular grid
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* gridSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new gridSampler(SamplerParams) ;
}

gridSampler::gridSampler(const vector<string>& SamplerParams)
{
    SamplingType = "Grid" ;
}

void gridSampler::MTSample(vector<Point2d>& pts, int n) const
{
    int sqrtN (ceil(sqrt(n))) ;
    double dX(1.0f/(sqrtN)), dY(dX);

    pts.resize(n) ;

    #pragma omp parallel for
    for (int i=0; i<sqrtN; i++)
    {
    for (int j=0; j<sqrtN; j++)
    {
        const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
        pts[i*sqrtN+j] =  Point2d(x,y) ;
    }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Gaussian jitter
////////////////////////////////////////////////////////////////////////////////////////////////////////////
const string gjSampler::SigStr = "--sigma" ;

Sampler* gjSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new gjSampler(SamplerParams) ;
}

gjSampler::gjSampler(const vector<string>& SamplerParams) : _sigma (0.5)
{
    SamplingType = "GJittered" ;
    ParseParameters(SamplerParams) ;
}

void gjSampler::ParseParameters(const vector<string>& SamplerParams)
{
    _sigma  = CLParser::FindArgument<double>(SamplerParams, SigStr) ;
}

void gjSampler::MTSample(vector<Point2d>& pts, int n) const
{
    int sqrtN (floor(sqrt(n))) ;
    double dX(1.0f/(sqrtN)), dY(dX);
    pts.resize(n) ;
    std::normal_distribution<double> ND(0,1);

    #pragma omp parallel for
    for (int i=0; i<sqrtN; i++)
    {
    for (int j=0; j<sqrtN; j++)
    {
        const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
        const double r1(ND(RGen)*dX*.5), r2(ND(RGen)*dX*.5);
        pts[i*sqrtN+j] = Point2d(x+(r1),y+(r2), true);
    }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Box-jitter
////////////////////////////////////////////////////////////////////////////////////////////////////////////
const string bjSampler::BWStr = "--boxwidth" ;
Sampler* bjSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new bjSampler(SamplerParams) ;
}

bjSampler::bjSampler(const vector<string>& SamplerParams) : _boxWidth(0.5)
{
    SamplingType = "BJittered" ;
    ParseParameters(SamplerParams) ;
}

void bjSampler::ParseParameters(const vector<string>& SamplerParams)
{
    _boxWidth = CLParser::FindArgument<double>(SamplerParams, BWStr) ;
}

void bjSampler::MTSample(vector<Point2d>& pts, int n) const
{
    int sqrtN (floor(sqrt(n))) ;
    double dX(1.0f/(sqrtN)), dY(dX);
    pts.resize(n) ;

    #pragma omp parallel for
    for (int i=0; i<sqrtN; i++)
    {
    for (int j=0; j<sqrtN; j++)
    {
        const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
        const double r1(-.5+drand48()), r2(-.5+drand48());
        pts[i*sqrtN+j] = Point2d(x+(r1)*_boxWidth*dX,y+(r2)*_boxWidth*dY, true);
    }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				LatinHyperCube
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* latinhypercubeSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new latinhypercubeSampler(SamplerParams) ;
}

latinhypercubeSampler::latinhypercubeSampler(const vector<string>& SamplerParams)
{
    SamplingType = "latinhypercube" ;
}

void latinhypercubeSampler::MTSample(vector<Point2d>& pts, int n) const
{
    for (int i=0; i<n; i++)
        pts.push_back(Point2d(0.,0.));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Halton
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* haltonSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new haltonSampler(SamplerParams) ;
}

haltonSampler::haltonSampler(const vector<string>& SamplerParams)
{
    SamplingType = "halton" ;
}

void haltonSampler::MTSample(vector<Point2d>& pts, int n) const
{
    for (int i=0; i<n; i++)
        pts.push_back(Point2d(0.,0.));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Sobol
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* sobolSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new sobolSampler(SamplerParams) ;
}

sobolSampler::sobolSampler(const vector<string>& SamplerParams)
{
    SamplingType = "sobol" ;
}

void sobolSampler::MTSample(vector<Point2d>& pts, int n) const
{
    for (int i=0; i<n; i++)
        pts.push_back(Point2d(0.,0.));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				02sequence
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* zerotwosequenceSampler::GenSampler(const vector<string>& SamplerParams)
{
    return new zerotwosequenceSampler(SamplerParams) ;
}

zerotwosequenceSampler::zerotwosequenceSampler(const vector<string>& SamplerParams)
{
    SamplingType = "02sequence" ;
}

void zerotwosequenceSampler::MTSample(vector<Point2d>& pts, int n) const
{
    for (int i=0; i<n; i++)
        pts.push_back(Point2d(0.,0.));
}




