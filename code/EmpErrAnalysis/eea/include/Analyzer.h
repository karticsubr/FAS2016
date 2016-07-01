#ifndef __ANALYZERH__
#define __ANALYZERH__

#include <vector>
#include <string>
#include <DataMat.h>>

using std::string ;
using std::cout ;
using std::endl ;
using std::vector ;

class Analyzer 
{
    public:
	Analyzer(int argc, char* argv[]) ;
	
	void AssessAll() ;

	void AssessSelectedIntegrand(int integrandID) ;

	void AssessSelectedSampler(int samplerID) ;
	
	void AssessSelectedSamplerIntegrand(int samplerID, int integrandID) ;

	void WriteResults(const string& path) const ;
	
    private:
	int _ns, _ni ; // numbers of samplers and integrands (combinations) to be analysed
	    
	void Parse(int argc, char* argv[]) ;
	
	DataMat _nSamples ; // samplers (rows) will use different sets samples (columns) 
	
	vector<Sampler*> _vs ;
	vector<SamplerParams> _vsprm ;

	vector<Integrand*> _vi ;
	vector<IntegrandParams> _vsprm ;
	
	DataMat _conv, _vconv, _maxErr ; // after fitting best fit lines to data
};
#endif //__ANALYZERH__