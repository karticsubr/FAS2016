#include <samples.h>
#include <omp.h>
#include <randsampgsl.h>
#define NDIM 2
// #include "generator.h"
#include <stdexcept>
#include <unistd.h>


// using namespace boost; 
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
    Sample(n, param) ;
}

vector<Point2D>& randomSampler::Sample(int n, const SamplerParams& param) 
{
    #pragma omp parallel for
    for (int i=0; i<n; i++)
    {
	Point2D p2(drand48(),drand48()) ;
	p.push_back(p2) ;
    }
    return p;
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
    Sample(n, param) ;
}

vector<Point2D>& gridSampler::Sample(int n, const SamplerParams& param) 
{
    double sqrtN (ceil(sqrt(n))) ;
     double dX(1/(sqrtN)), dY(dX) ;
    
    #pragma omp parallel for
    for (int i=0; i<sqrtN; i++)
    {
	for (int j=0; j<sqrtN; j++)
	{
	    const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
	    p.push_back(Point2D(x,y)) ;
	}
    }
    return p;
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
    Sample(n, param) ;
}

vector<Point2D>& jitteredSampler::Sample(int n, const SamplerParams& param) 
{
    double sqrtN (ceil(sqrt(n))) ;
    double dX(1/(sqrtN)), dY(dX);
    
  #pragma omp parallel for
    for (int i=0; i<sqrtN; i++)
    {
	for (int j=0; j<sqrtN; j++)
	{
	    const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
	    const Point2D pt(x+(drand48()-.5)*dX,y+(drand48()-.5)*dY) ;
	    p.push_back(pt) ;
	}
    }
    return p;
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
    Sample(n, param) ;
}

vector<Point2D>& gjSampler::Sample(int n, const SamplerParams& param) 
{
    double sqrtN (ceil(sqrt(n))) ;
    double dX(1/(sqrtN)), dY(dX);
    
    #pragma omp parallel for
    for (int i=0; i<sqrtN; i++)
    {
	for (int j=0; j<sqrtN; j++)
	{
	    const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
	    const double r1(gslSamps.GaussianRandSample(param.sigma*dX*.5)), r2(gslSamps.GaussianRandSample(param.sigma*dY*.5)); 
	    Point2D pt(x+(r1),y+(r2), true);
	    p.push_back(pt) ;
	}
    }
    return p;
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
    Sample(n, param) ;
}

vector<Point2D>& bjSampler::Sample(int n, const SamplerParams& param) 
{
    double sqrtN (ceil(sqrt(n))) ;
    double dX(1/(sqrtN)), dY(dX);
    
    #pragma omp parallel for
    for (int i=0; i<sqrtN; i++)
    {
	for (int j=0; j<sqrtN; j++)
	{
	    const double x(dX/2.0 + i*dX), y(dY/2.0 + j*dY) ;
	    const double r1(-.5+drand48()), r2(-.5+drand48()); 
	    Point2D pt(x+(r1)*param.sigma*dX,y+(r2)*param.sigma*dY, true);	    
	    p.push_back(pt) ;
	}
    }
    return p;
}
