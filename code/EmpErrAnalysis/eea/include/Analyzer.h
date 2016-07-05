#ifndef __ANALYZERH__
#define __ANALYZERH__

#include <vector>
#include <string>
#include <DataMat.h>

using std::string ;
using std::cout ;
using std::endl ;
using std::vector ;

class Sampler ;
class Integrand; 

class Analyzer 
{
    public:
	Analyzer(Sampler* s, Integrand* i, const vector<string> asec) ;
	
	void RunAnalysis() ;
	
	double GetConvergenceRate() const {return _convRate;}
	double GetYInterceptError() const {return _YIntError ;}

	void WriteResults(const string& path) const ;
	
    private:

	
	static const string NSampStr; // --nsamps
	vector<int> _nSamples ; // secondary estimator sample sizes (for convergence)
	
	static const string NRepsStr; // --nreps
	int _nReps ; // iterations  
	
	static const string AnalTypeStr; // --atype ( 'err'<default> 'var') 
	string _atype ;
	
	Sampler* _sampler ;
	Integrand* _integrand ; 
	
	vector<double> _avgM, _avgV, _MSE ;

	double _convRate, _YIntError ;
	
};
#endif //__ANALYZERH__