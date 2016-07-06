

#include <integrand.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <omp.h>
#include <cmdlnparser.h>


#include <PWConstIntegrand.h>
#include <QPIntegrand.h>

using std::vector ;
using std::map ;
using std::cout ;
using std::endl ;
using std::invalid_argument ;


IntegrandPrototype::IntegrandPrototype()
{
    vector<Integrand*> vi ;
    vi.push_back(new QuadPixelIntegrand());
    vi.push_back(new PWConstantIntegrand());
    
    for (int i(0); i<vi.size(); i++)
	    exemplars[vi[i]->GetType()] = vi[i] ;
}


map<string, Integrand*> IntegrandPrototype::exemplars ;
IntegrandPrototype i1;  

Integrand* IntegrandPrototype::Generate(const vector<string>& IntegSection) 
{
	string type = CLParser::FindArgument<string>(IntegSection, CLArg::IntegrandType) ;

	map<string, Integrand*>::iterator it = exemplars.find(type) ;
	if (it==exemplars.end()) 
		throw invalid_argument("Unknown integrand type") ;
	return it->second->GenIntegrand(IntegSection) ;
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
































































