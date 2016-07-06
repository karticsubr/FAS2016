#ifndef __INTEGRANDH_ 
#define __INTEGRANDH_ 

#include <point2d.h>
#include <vector>
#include <map> 
#include <string> 

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

	inline double MyRandom()
	{
		static const double invrmax(1/static_cast<double>(RAND_MAX)) ;
		return rand()*invrmax ; 
	}


#endif //__INTEGRANDH_ 