#include <PWConstIntegrand.h>
#include <cmdlnparser.h>

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

	_npts = CLParser::FindArgument<double>(IntegParams, NPtsStr) ;
	
	vector<Point> vp(_npts+4) ;
	vp[0] =  Point(0, 0) ;
	vp[1] =  Point(0, 1) ;
	vp[2] =  Point(1, 0) ;
	vp[3] =  Point(1, 1) ;
	for (int i(0); i<_npts; i++)
		vp[i+4] = Point(MyRandom(), MyRandom()) ;
	
	_dt.insert(vp.begin(), vp.end()) ;
	

	for(Finite_faces_iterator fc = _dt.finite_faces_begin(); fc != _dt.finite_faces_end(); ++fc) 
		fc->info() = MyRandom();
	
}
