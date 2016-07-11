#include <Analyzer.h>
#include <cmdlnparser.h>
#include <integrand.h>
#include <iostream>
#include <samples.h>
#include <integrand.h>
#include <fstream>
#include <omp.h>

using namespace std;


const string Analyzer::NSampStr = "--nsamps" ;
const string Analyzer::NRepsStr = "--nreps" ;
const string Analyzer::AnalTypeStr = "--atype" ;

namespace
{
	inline double Mean(const vector<double>& v)
	{
		double m(0);
		for(int i(0); i<v.size(); i++) m+=v[i] ;
		return m/v.size() ;
	}	

	inline double Var(const vector<double>& v)
	{
		double m(Mean(v));
		double var(0);
		for(int i(0); i<v.size(); i++) var+=(v[i]-m)*(v[i]-m) ;
		return var/v.size() ;
	}	

	inline void MeanAndVar(const vector<double>& v, double& m, double& var)
	{
		m=0;
		var=0;
		for(int i(0); i<v.size(); i++) m+=v[i] ;
		m/=v.size() ;
		
		for(int i(0); i<v.size(); i++) var+=(v[i]-m)*(v[i]-m) ;
		var/=v.size() ;
	}	
	
	inline void LogLogLinearFit(const vector<int>& x, const vector<double>& y, double& m, double& b)
	{
		double Xsum(0), X2sum(0), Ysum(0), XYsum(0) ;
		for(int i(0); i<y.size(); i++) 
		{
			const double lx (log(static_cast<double>(x[i]))) ;
			const double ly (log(y[i])) ;
			
			Xsum += lx ;
			Ysum += ly ;
			X2sum += lx*lx ;
			XYsum += lx*ly ;
		}
		
		const int n(x.size()) ;
		m = (n*XYsum - Xsum*Ysum) / (n*X2sum - Xsum*Xsum)  ;
		b=(X2sum*Ysum-Xsum*XYsum)/(X2sum*n-Xsum*Xsum);
	}
}

Analyzer::Analyzer(Sampler* s, Integrand* i, const vector<string> asec) :_sampler(s), _integrand(i), _atype("err") 
{
	_atype = CLParser::FindArgument<string>(asec, AnalTypeStr) ;
	_nReps = CLParser::FindArgument<int>(asec, NRepsStr) ;
	CLParser::FindMultiArgs<int>(-1, _nSamples, asec, NSampStr) ;
	
// 	cout << _atype << " with " << _nReps << " reps and #samps: " << flush ;
// 	copy(_nSamples.begin(), _nSamples.end(), ostream_iterator<int>(cout, " ")); 
// 	cout << endl ;
}


void Analyzer::WriteResults(const string& path) const 
{
	ofstream ofs(path.c_str(), ofstream::app) ;
	
	copy(_nSamples.begin(), _nSamples.end(), ostream_iterator<int>(ofs, " ")); 
	
	if (_atype=="var")
		copy(_avgV.begin(), _avgV.end(), ostream_iterator<double>(ofs, " ")); 
	else if (_atype=="err")
		copy(_MSE.begin(), _MSE.end(), ostream_iterator<double>(ofs, " ")); 
	
	ofs << endl ;
}


void Analyzer::RunAnalysis() 
{
	const double Iref (_integrand->ReferenceValue()) ;
	_avgM.resize(_nSamples.size()) ;
	_avgV.resize(_nSamples.size()) ;
	_MSE.resize(_nSamples.size()) ;
	
	for (int i=0; i<_nSamples.size(); i++)
	{
		_avgM[i]=0;
		_avgV[i]=0;
		_MSE[i]=0 ;
	}

	#pragma omp parallel for
	for (int i=0; i<_nSamples.size(); i++)
	{
		const int n(_nSamples[i]) ;
		vector<double> ms(_nReps,0) ;
		
		for (int r=0; r<_nReps; r++)
		{
			vector<Point2D> S; 
			_sampler->MTSample(S, n) ;
			
			vector<double> res ;
			_integrand->MultipointEval(res, S) ;
			double m = Mean(res); 
			ms[r] = m ;
			_avgM[i] += m ;
			_MSE[i] += (Iref-m)*(Iref-m) ;
		}
		_avgV[i] = Var(ms) ;
		_avgM[i] /= _nReps ;
		_MSE[i] /= _nReps ;
	}

	if (_atype=="var")
	{
		LogLogLinearFit(_nSamples, _avgV, _convRate, _YIntError);
		_YIntError = exp(_YIntError); 
// 		cout << " Conv rate and Y-intercept (var): " << _convRate << " " << _YIntError << endl ;
	}
	else if (_atype=="err")
	{
		LogLogLinearFit(_nSamples, _MSE, _convRate, _YIntError);
// 		cout << " Conv rate and Y-intercept (MSE): " << _convRate << " " << _YIntError << endl ;
	}
}

