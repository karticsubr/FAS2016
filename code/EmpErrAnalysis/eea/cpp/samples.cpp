#include <samples.h>
#include <omp.h>
#include <randsampgsl.h>
#define NDIM 2
#include <stdexcept>
#include <unistd.h>
#include <cmdlnparser.h>
#include <sstream>

#include <iostream>
// using namespace boost; 
using std::cout ;
using std::stringstream ;
using std::endl ;
using std::invalid_argument ;

extern RandSampGSL gslSamps;

map<string, Sampler*> SamplerPrototype::exemplars ;
SamplerPrototype s1;  


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
Sampler* SamplerPrototype::Generate(const vector<string>& SamplerString) 
{
	string type = CLParser::FindArgument<string>(SamplerString, CLArg::SamplerType) ;
	
	map<string, Sampler*>::iterator it = exemplars.find(type) ;
	if (it==exemplars.end()) throw invalid_argument("Unknown sampler type") ;
	return it->second->GenSampler(SamplerString) ;
}

SamplerPrototype::SamplerPrototype()
{
    vector<Sampler*> vs ;
    vs.push_back(new randomSampler());
    vs.push_back(new gridSampler());
    vs.push_back(new jitteredSampler());
    vs.push_back(new gjSampler());
    vs.push_back(new bjSampler());
    
    for (int i(0); i<vs.size(); i++)
	exemplars[vs[i]->GetType()] = vs[i] ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
ostream& operator << (ostream& os, Sampler& s)
{
    for (int i(0); i< s.p.size(); i++)
	os << s.p[i].x << " " << s.p[i].y << endl ; 
}


vector<Point2D>& Sampler::Sample(int n) 
{
	MTSample(p, n) ;
	return p;
}

Sampler::~Sampler() 
{}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Random
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* randomSampler::GenSampler(const vector<string>& SamplerParams)  
{
    return new randomSampler(SamplerParams) ;
}

randomSampler::randomSampler(const vector<string>& SamplerParams) 
{
    SamplingType = "Random" ;
}

void randomSampler::MTSample(vector<Point2D>& pts, int n) const 
{
    pts.resize(n) ;
    #pragma omp parallel for
    for (int i=0; i<n; i++)
    {
	pts[i] = Point2D(drand48(),drand48()) ;
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

void gridSampler::MTSample(vector<Point2D>& pts, int n) const 
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
	    pts[i*sqrtN+j] =  Point2D(x,y) ;
	}
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Jittered
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* jitteredSampler::GenSampler(const vector<string>& SamplerParams)  
{
    return new jitteredSampler(SamplerParams) ;
}

jitteredSampler::jitteredSampler(const vector<string>& SamplerParams) 
{
    SamplingType = "Jittered" ;
}

void jitteredSampler::MTSample(vector<Point2D>& pts, int n) const 
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
	    pts[i*sqrtN+j] = Point2D(x+(drand48()-.5)*dX,y+(drand48()-.5)*dY) ;
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
	cout << "===============>"  << _sigma << endl ;
}

void gjSampler::MTSample(vector<Point2D>& pts, int n) const 
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
	    const double r1(gslSamps.GaussianRandSample(_sigma*dX*.5)), r2(gslSamps.GaussianRandSample(_sigma*dY*.5)); 
	    pts[i*sqrtN+j] = Point2D(x+(r1),y+(r2), true);
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

void bjSampler::MTSample(vector<Point2D>& pts, int n) const 
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
	    const double r1(-.5+drand48()), r2(-.5+drand48()); 
	    pts[i*sqrtN+j] = Point2D(x+(r1)*_boxWidth*dX,y+(r2)*_boxWidth*dY, true);
	}
    }
}
