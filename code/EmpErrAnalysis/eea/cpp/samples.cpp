#include <samples.h>
#include <omp.h>
#include <randsampgsl.h>
#define NDIM 2
// #include "generator.h"

// using namespace boost; 

extern RandSampGSL gslSamps;

map<string, samples*> sampler::exemplars ;
sampler s1;  

samples* sampler::Generate(const string& type, int n,  const Params& param) 
{
//     cout << "# exemplars = " << exemplars.size() << endl ;
    return exemplars[type]->GenSamples(n, param) ;
}

sampler::sampler()
{
    vector<samples*> vs ;
    vs.push_back(new randomSamples());
    vs.push_back(new gridSamples());
    vs.push_back(new jitteredSamples());
    vs.push_back(new gjSamples());
    vs.push_back(new bjSamples());
    
    for (int i(0); i<vs.size(); i++)
	exemplars[vs[i]->GetType()] = vs[i] ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
ostream& operator << (ostream& os, samples& s)
{
    for (int i(0); i< s.p.size(); i++)
	os << s.p[i].x << " " << s.p[i].y << endl ; 
}

void samples::WriteEPSFile(const string& fname) 
{
}

samples::~samples() 
{}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

samples* randomSamples::GenSamples(int n,  const Params& param)  
{
    return new randomSamples(n, param) ;
}

randomSamples::randomSamples(int n,  const Params& param) 
{
    #pragma omp parallel for
    for (int i=0; i<n; i++)
    {
	Point2D p2(drand48(),drand48()) ;
	p.push_back(p2) ;
    }
    SamplingType = "Random" ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

samples* gridSamples::GenSamples(int n,  const Params& param)  
{
    return new gridSamples(n, param) ;
}

gridSamples::gridSamples(int n,  const Params& param) 
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
    SamplingType = "Grid" ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

samples* jitteredSamples::GenSamples(int n,  const Params& param)  
{
    return new jitteredSamples(n, param) ;
}

jitteredSamples::jitteredSamples(int n,  const Params& param) 
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
    SamplingType = "Jittered" ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

samples* gjSamples::GenSamples(int n,  const Params& param)  
{
    return new gjSamples(n, param) ;
}

gjSamples::gjSamples(int n,  const Params& param) 
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
    SamplingType = "GJittered" ;
}

samples* bjSamples::GenSamples(int n,  const Params& param)  
{
    return new bjSamples(n, param) ;
}

bjSamples::bjSamples(int n,  const Params& param) 
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
    SamplingType = "BJittered" ;
}
