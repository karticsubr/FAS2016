#ifndef __INTEGRANDH_ 
#define __INTEGRANDH_ 

#include <point2d.h>
#include <vector>
#include <map> 
#include <string> 

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Triangulation_euclidean_traits_2.h>
#include <CGAL/Triangulation_2.h>
#include <fstream>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<double,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Triangulation_2<K,Tds> Triangulation;

// typedef CGAL::Triangulation_euclidean_traits_2<K> Gt;
// typedef CGAL::Triangulation_2<Gt, Fb>  Delaunay;
typedef K::Point_2 Point;

using std::vector ;
using std::map ;
using std::string ;  

class Integrand ;


///////////////////////////////////////////////

class IntegrandPrototype
{
    public:
	IntegrandPrototype() ;

	static Integrand* Generate(const vector<string>& IntegSection) ;
	
    private:
	static map<string, Integrand*> exemplars;
};

///////////////////////////////////////////////

class Integrand
{
     public:
	virtual Integrand* GenIntegrand(const vector<string>& IntegParams) = 0 ;
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
// Param settings 
//   1) 4 floats to choose points on L, T, R, B of [0,1]x[0,1]
//   2) RandomPoints=true overrides above 
// 
// Evaluation returns 1 if inside the quad and 0 outside
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class QuadPixelIntegrand : public Integrand
{
     public:
	virtual Integrand* GenIntegrand(const vector<string>& IntegParams) ;
	virtual double operator () (const Point2D& p) const ;
	virtual ~QuadPixelIntegrand() ;
	
     protected:
	QuadPixelIntegrand(): _randomize(false) { RefVal = 0 ; IntegrandType = "QuadPix"; }
	QuadPixelIntegrand(const vector<string>& IntegParams);
	friend class IntegrandPrototype;
	
     private:
	Point pts[4]; 
	static const string PointsStr ; // = "--points" 

	
	bool _randomize ;
	static const string RandStr ; // = "--rand" 
} ;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 			Piecewise constant simplicial complex in 2D
// Defined by
//    1) random points within a [0,1]x[0,1] quad + 4 corners of the quad
//    2) random weights within the triangles [0,1]
// Params 
//   1) --npts : number of points within the quad
// 
// Evaluation returns weight of the corresponding triangle 
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class PWConstantIntegrand : public Integrand
{
     public:
	virtual Integrand* GenIntegrand(const vector<string>& IntegParams) ;
	virtual double operator () (const Point2D& p) const ;
	virtual ~PWConstantIntegrand() ;
	
     protected:
	PWConstantIntegrand(): _randomize(true) { RefVal = 0 ; IntegrandType = "PWConstant"; }
	PWConstantIntegrand(const vector<string>& IntegParams);
	friend class IntegrandPrototype;
	
     private:
	int _npts; 
	static const string NPtsStr ; // = "--points" 

	
	bool _randomize ;
	static const string RandStr ; // = "--rand" 
	
	Triangulation _dt;
	vector<double> _wts ;
} ;

#endif //__INTEGRANDH_ 