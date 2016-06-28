

#include <point2d.h>
#include <integrand.h>
#include <vector>
#include <map>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <omp.h>

using std::vector ;
using std::map ;
using std::cout ;
using std::endl ;
using std::invalid_argument ;


namespace
{	
}

map<string, Integrand*> IntegrandPrototype::exemplars ;
IntegrandPrototype i1;  

Integrand* IntegrandPrototype::Generate(const string& type, const IntegrandParams& param) 
{
	map<string, Integrand*>::iterator it = exemplars.find(type) ;
	if (it==exemplars.end()) throw invalid_argument("Unknown integrand type") ;
	return it->second->GenIntegrand(param) ;
}

IntegrandPrototype::IntegrandPrototype()
{
    vector<Integrand*> vi ;
    vi.push_back(new QuadPixelIntegrand());
    
    for (int i(0); i<vi.size(); i++)
	    exemplars[vi[i]->GetType()] = vi[i] ;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Base class
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Integrand::MultipointEval (vector<double>& out, const vector<Point2D>& vp) const 
{
	const int n (vp.size());
	out.resize(n) ;
	
	#pragma omp parallel for
	for (int i=0; i<n; i++)
	{
// 		std::cout << "\t\tThread number: " << omp_get_thread_num() << endl;			
		out[i] = (*this)(vp[i]) ;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				White quad within black pixel
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Integrand* QuadPixelIntegrand::GenIntegrand(const IntegrandParams& params) 
{
	return new QuadPixelIntegrand(params) ;
}

QuadPixelIntegrand::QuadPixelIntegrand(const IntegrandParams& params)
{
	IntegrandType = "QuadPix"; 

	// initialise points
	double f[4] ;
	Randomize = params.RandomPoints || (params.Floats.size()!=4) ;
	double invrmax(1/static_cast<double>(RAND_MAX)) ;
	for (int i(0); i<4;i++)
		f[i] = Randomize ? (rand()*invrmax):params.Floats[i] ;
	pts[0] = Point(0,f[0]) ;
	pts[1] = Point(f[1],1) ;
	pts[2] = Point(1,1-f[2]) ;
	pts[3] = Point(1-f[3],0) ;
	
	RefVal = 1 - 0.5f*(f[0] + f[1] + f[2] + f[3] - (f[0]*f[1] + 
				f[1]*f[2] + f[2]*f[3] + f[3]*f[0]) ) ;
	
	cout << "Refval = " << RefVal << endl ;	
}

double QuadPixelIntegrand::operator () (const Point2D& p) const 
{
	double eval(0); 
	Point pt(p.x, p.y) ;
	CGAL::Bounded_side test = CGAL::bounded_side_2(pts, pts+4, pt, K()) ;
	eval =  (test == CGAL::ON_BOUNDED_SIDE) ? 1: 0 ;
	return eval ;
}

QuadPixelIntegrand::~QuadPixelIntegrand() 
{}