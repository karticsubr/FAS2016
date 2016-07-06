#ifndef __PWCINTEGRANDH_ 
#define __PWCINTEGRANDH_ 

#include <integrand.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_euclidean_traits_2.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>


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
	typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
	typedef CGAL::Triangulation_vertex_base_2<K> Vb;
	typedef CGAL::Triangulation_face_base_with_info_2<double,K> Fb;
	typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
	typedef CGAL::Triangulation_2<K,Tds> Triangulation;
	typedef K::Point_2 Point;
	typedef Triangulation::Face_handle fh;
	typedef Triangulation::Finite_faces_iterator Finite_faces_iterator;

	int _npts; 
	static const string NPtsStr ; // = "--points" 

	
	bool _randomize ;
	static const string RandStr ; // = "--rand" 
	
	Triangulation _dt;
	vector<double> _wts ;
} ;

#endif