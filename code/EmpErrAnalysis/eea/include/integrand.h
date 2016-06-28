#ifndef __INTEGRANDH_ 
#define __INTEGRANDH_ 

#include <point2d.h>
#include <vector>
#include <map> 
#include <string> 

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2_algorithms.h>

using std::vector ;
using std::map ;
using std::string ;  

class Integrand ;

///////////////////////////////////////////////
struct IntegrandParams
{
	vector<double> Floats;
	vector<Point2D> Points ;
	bool RandomPoints; 
};

///////////////////////////////////////////////

class IntegrandPrototype
{
    public:
	IntegrandPrototype() ;

	static Integrand* Generate(const string& type, const IntegrandParams& param) ;
	static map<string, Integrand*> exemplars;
};

///////////////////////////////////////////////

class Integrand
{
     public:
	virtual Integrand* GenIntegrand(const IntegrandParams& params) = 0 ;
	virtual double operator () (const Point2D& p) const = 0;
	virtual void MultipointEval (vector<double>& out, const vector<Point2D>& vp) const ;
	virtual double ReferenceValue() const {return RefVal;} 
	virtual string GetType() const {return IntegrandType; } 
	virtual ~Integrand() {} 
	
     protected:
	double RefVal ;
	string IntegrandType ;

} ;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				White quad within black pixel
// Quad defined by
//    1) 4 points on the left, top, right and bottom edge of a [0,1]x[0,1] pixel
//    2) 4 random values along pixel edges, connected to form a quad
// 
// IntegrandParams settings 
//   1) f[0] ... f[3] 
//   2) RandomPoints=true overrides above 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;

class QuadPixelIntegrand : public Integrand
{
     public:
	virtual Integrand* GenIntegrand(const IntegrandParams& params) ;
	virtual double operator () (const Point2D& p) const ;
	virtual ~QuadPixelIntegrand() ;
	
     protected:
	QuadPixelIntegrand(): Randomize(false) { RefVal = 0 ; IntegrandType = "QuadPix"; }
	QuadPixelIntegrand(const IntegrandParams& params);
	friend class IntegrandPrototype;
	
     private:
	Point pts[4]; 
	bool Randomize ;
} ;


#endif //__INTEGRANDH_ 