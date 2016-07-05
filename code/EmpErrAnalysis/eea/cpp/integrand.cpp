

#include <point2d.h>
#include <integrand.h>
#include <vector>
#include <map>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <omp.h>
#include <cmdlnparser.h>

typedef Triangulation::Face_handle fh;
typedef Triangulation::Finite_faces_iterator Finite_faces_iterator;

using std::vector ;
using std::map ;
using std::cout ;
using std::endl ;
using std::invalid_argument ;


namespace
{	
	inline double MyRandom()
	{
		static const double invrmax(1/static_cast<double>(RAND_MAX)) ;
		return rand()*invrmax ; 
	}
}

map<string, Integrand*> IntegrandPrototype::exemplars ;
IntegrandPrototype i1;  

Integrand* IntegrandPrototype::Generate(const vector<string>& IntegSection) 
{
	string type = CLParser::FindArgument<string>(IntegSection, CLArg::IntegrandType) ;

	map<string, Integrand*>::iterator it = exemplars.find(type) ;
	if (it==exemplars.end()) throw invalid_argument("Unknown integrand type") ;
	return it->second->GenIntegrand(IntegSection) ;
}

IntegrandPrototype::IntegrandPrototype()
{
    vector<Integrand*> vi ;
    vi.push_back(new QuadPixelIntegrand());
    vi.push_back(new PWConstantIntegrand());
    
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

Integrand* QuadPixelIntegrand::GenIntegrand(const vector<string>& IntegParams) 
{
	return new QuadPixelIntegrand(IntegParams) ;
}

const string QuadPixelIntegrand::RandStr = "--random" ; 
const string QuadPixelIntegrand::PointsStr = "--points" ; 

QuadPixelIntegrand::QuadPixelIntegrand(const vector<string>& IntegParams)
{
	IntegrandType = "QuadPix"; 

	_randomize = CLParser::FindSwitch(IntegParams, RandStr) ;

	vector<double> MultiArg;
	CLParser::FindMultiArgs<double>(4, MultiArg, IntegParams, PointsStr) ;
	
	
	// initialise points
	double f[4] ;
	for (int i(0); i<4;i++)
		f[i] = _randomize ? MyRandom():MultiArg[i] ;
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 			Piecewise constant simplicial complex in 2D
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Integrand* PWConstantIntegrand::GenIntegrand(const vector<string>& IntegParams) 
{
	return new PWConstantIntegrand(IntegParams) ;	
}

const string PWConstantIntegrand::RandStr = "--random" ; 
const string PWConstantIntegrand::NPtsStr = "--npts" ; 

double PWConstantIntegrand::operator () (const Point2D& p) const 
{
	return  _dt.locate(Point(p.x, p.y))->info() ;
}

PWConstantIntegrand::~PWConstantIntegrand() 
{}
	
PWConstantIntegrand::PWConstantIntegrand(const vector<string>& IntegParams)
{
	IntegrandType = "PWConstant"; 

	_randomize=true ; // for now this is the only option
// 	_randomize = CLParser::FindSwitch(IntegParams, RandStr) ;

	_npts = CLParser::FindSwitch(IntegParams, NPtsStr) ;
	
	vector<Point> vp(_npts+4) ;
	vp[0] =  Point(0, 0) ;
	vp[1] =  Point(0, 1) ;
	vp[2] =  Point(1, 0) ;
	vp[3] =  Point(1, 1) ;
	for (int i(0); i<_npts; i++)
	{
		vp[i+4] = Point(MyRandom(), MyRandom()) ;
	}
	_dt.insert(vp.begin(), vp.end()) ;
	

	for(Finite_faces_iterator fc = _dt.finite_faces_begin(); fc != _dt.finite_faces_end(); ++fc) 
		fc->info() = MyRandom();
	
}































































