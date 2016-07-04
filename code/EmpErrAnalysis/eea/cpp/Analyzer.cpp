#include <Analyzer.h>
#include <cmdlnparser.h>
#include <integrand.h>
#include <iostream>
#include <samples.h>
#include <integrand.h>

using namespace std;


const string Analyzer::NSampStr = "--nsamps" ;
const string Analyzer::NRepsStr = "--nreps" ;
const string Analyzer::AnalTypeStr = "--atype" ;

namespace
{
	inline void MeanAndVar(const vector<double>& v, double& m, double& var)
	{
		m=0;
		var=0;
		for(int i(0); i<v.size(); i++) m+=v[i] ;
		m/=v.size() ;
		
		for(int i(0); i<v.size(); i++) var+=(v[i]-m)*(v[i]-m) ;
		var/=v.size() ;
	}	
}

Analyzer::Analyzer(Sampler* s, Integrand* i, const vector<string> asec) :_sampler(s), _integrand(i)
{
	_atype = CLParser::FindArgument<string>(asec, AnalTypeStr) ;
	_nReps = CLParser::FindArgument<int>(asec, NRepsStr) ;
	CLParser::FindMultiArgs<int>(-1, _nSamples, asec, NSampStr) ;
	
	cout << _atype << " with " << _nReps << " reps and #samps: " << flush ;
	copy(_nSamples.begin(), _nSamples.end(), ostream_iterator<int>(cout, " ")); 
	cout << endl ;
}

	
void Analyzer::WriteResults(const string& path) const 
{
}


void Analyzer::RunAnalysis() 
{
	const double Iref (_integrand->ReferenceValue()) ;
	_avgM.resize(_nSamples.size()) ;
	_avgV.resize(_nSamples.size()) ;
	
	for (int i=0; i<_nSamples.size(); i++)
	{
		const int n(_nSamples[i]) ;
		for (int r=0; r<_nReps; r++)
		{
			vector<Point2D> S; 
			_sampler->MTSample(S, n) ;
			
			vector<double> res ;
			_integrand->MultipointEval(res, S) ;
			
			double m(0), v(0); 
			MeanAndVar(res, m, v); 
			
			_avgM[i] += m ;
			_avgV[i] += v ;
		}
		_avgM[i] /= _nReps ;
		_avgV[i] /= _nReps ;
	}

// 	cout << " === " << endl ;
// 	copy(_avgM.begin(), _avgM.end(), ostream_iterator<double>(cout, " ")); 
// 	cout << endl ;
// 	copy(_avgV.begin(), _avgV.end(), ostream_iterator<double>(cout, " ")); 
// 	cout << " === " << endl ;

	
	
}

