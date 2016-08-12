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

///////////////////////////////////////////////
// Analyzer class
// requires (pointers to) sampler and integrand objects 
// and the portion of the commandline meant for the analyzer
// 
// It uses the sampler and the integrand to estimate the estimator's 
// convergence rate and the y-intercept of the convergence plot (log-log)
///////////////////////////////////////////////

// expected format for the analysis section is
// -A --nsamps n1 n2 n3 n4 ... --nreps r --atype a
// 
// n1 n2 n3 n4 are integers eg. 10 100 500 1000 to be used as sample counts
// r is an integer specifying the number of n1-sample estimates to be averaged for computing
//    error at n1 (equal to number of n2-sample estimates to be averaged for error at n2, etc.)
// a is a string that can either be "var" or "err" and is used to either output the variance at n1, n2 ... 
// or the MSE at n1, n2, ... respectively
//
// convergence rate is the slope of the best-fit line to 
//  log([n1 n2 n3 n4]) vs var/err estimated at those sample counts points.
//
class Analyzer 
{
    public:
	Analyzer(Sampler* s, Integrand* i, const vector<string> asec) ;
	
	void RunAnalysis() ;
  
    void onlineAnalysis();
	
	double GetConvergenceRate() const {return _convRate;}
	double GetYInterceptError() const {return _YIntError ;}

	void WriteResults(const string& path) const ;
    void WriteMeanVar(const string& meanPath, const string& varPath) const ;
    std::string Atype() const { return _atype; }
	
    Sampler* GetAnalyzingSampler() { return _sampler; }
    
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