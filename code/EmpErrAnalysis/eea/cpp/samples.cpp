#include <samples.h>
#include <omp.h>
#include <randsampgsl.h>
#define NDIM 2
// #include "generator.h"
#include <stdexcept>
#include <unistd.h>

#include <iostream>
// using namespace boost; 
using std::cout ;
using std::endl ;
using std::invalid_argument ;

extern RandSampGSL gslSamps;

map<string, Sampler*> SamplerPrototype::exemplars ;
SamplerPrototype s1;  

Sampler* SamplerPrototype::Generate(const string& type, int n,  const SamplerParams& param) 
{
    	map<string, Sampler*>::iterator it = exemplars.find(type) ;
	if (it==exemplars.end()) throw invalid_argument("Unknown sampler type") ;
	return it->second->GenSampler(n, param) ;
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
ostream& operator << (ostream& os, Sampler& s)
{
    for (int i(0); i< s.p.size(); i++)
	os << s.p[i].x << " " << s.p[i].y << endl ; 
}


vector<Point2D>& Sampler::Sample(int n, const SamplerParams& param) 
{
	MTSample(p, n, param) ;
	return p;
}

Sampler::~Sampler() 
{}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Random
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* randomSampler::GenSampler(int n,  const SamplerParams& param)  
{
    return new randomSampler(n, param) ;
}

randomSampler::randomSampler(int n,  const SamplerParams& param) 
{
    SamplingType = "Random" ;
    MTSample(p, n, param) ;
}

void randomSampler::MTSample(vector<Point2D>& pts, int n, const SamplerParams& param) 
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

Sampler* gridSampler::GenSampler(int n,  const SamplerParams& param)  
{
    return new gridSampler(n, param) ;
}

gridSampler::gridSampler(int n,  const SamplerParams& param) 
{
    SamplingType = "Grid" ;
    MTSample(p, n, param) ;
}

void gridSampler::MTSample(vector<Point2D>& pts, int n, const SamplerParams& param) 
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

Sampler* jitteredSampler::GenSampler(int n,  const SamplerParams& param)  
{
    return new jitteredSampler(n, param) ;
}

jitteredSampler::jitteredSampler(int n,  const SamplerParams& param) 
{
    SamplingType = "Jittered" ;
    MTSample(p, n, param) ;
}

void jitteredSampler::MTSample(vector<Point2D>& pts, int n, const SamplerParams& param) 
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

Sampler* gjSampler::GenSampler(int n,  const SamplerParams& param)  
{
    return new gjSampler(n, param) ;
}

gjSampler::gjSampler(int n,  const SamplerParams& param) 
{
    SamplingType = "GJittered" ;
    MTSample(p, n, param) ;
}

void gjSampler::MTSample(vector<Point2D>& pts, int n, const SamplerParams& param) 
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
	    const double r1(gslSamps.GaussianRandSample(param.sigma*dX*.5)), r2(gslSamps.GaussianRandSample(param.sigma*dY*.5)); 
	    pts[i*sqrtN+j] = Point2D(x+(r1),y+(r2), true);
	}
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Box-jitter
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sampler* bjSampler::GenSampler(int n,  const SamplerParams& param)  
{
    return new bjSampler(n, param) ;
}

bjSampler::bjSampler(int n,  const SamplerParams& param) 
{
    SamplingType = "BJittered" ;
    MTSample(p, n, param) ;
}

void bjSampler::MTSample(vector<Point2D>& pts, int n, const SamplerParams& param) 
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
	    pts[i*sqrtN+j] = Point2D(x+(r1)*param.sigma*dX,y+(r2)*param.sigma*dY, true);
	}
    }
}
