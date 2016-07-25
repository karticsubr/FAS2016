#include <Analyzer.h>
#include <cmdlnparser.h>
#include <integrand.h>
#include <iostream>
#include <samples.h>
#include <integrand.h>
#include <fstream>

#if defined(_OPENMP)
#ifdef __APPLE__
#include <libiomp/omp.h>
#else
#include <omp.h>
#endif
#endif

using namespace std;


const string Analyzer::NSampStr = "--nsamps" ;
const string Analyzer::NRepsStr = "--nreps" ;
const string Analyzer::AnalTypeStr = "--atype" ;

namespace // some functions to compute simple statistics of vector<double>
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

namespace progressive {
  inline double MCEstimator(const vector<double>& v)
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
  
  inline double mean_progressive(double &mean, const double &integral, int trial)
  {
    if(trial == 0){
      std::cerr <<"Progressive mean trial index must start from 1, and not 0 !!!" << std::endl;
      exit(-2);
    }
    else{
      mean = ((trial-1)/double(trial)) * mean + (1/double(trial)) * integral;
    }
    
    return mean;
  }
  
  inline double variance_progressive(double &variance, const double &mean, const double &integralVal, int trial)
  {
    if(trial == 0){
      std::cerr <<"Progressive variance trial index must start from 1, and not 0 !!!" << std::endl;
      exit(-2);
    }
    else if(trial < 2){
      variance = 0;
    }
    else{
      double deviation = integralVal - mean;
      variance = ((trial-1)/double(trial)) * variance + (1/double(trial-1)) * deviation * deviation;
    }
    return variance;
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

void Analyzer::WriteMeanVar(const string &meanPath, const string &varPath) const
{
  ofstream ofsmean(meanPath.c_str(), ofstream::app) ;
  ofstream ofsvar(varPath.c_str(), ofstream::app) ;
  
  for(int i = 0; i < _nSamples.size(); i++){
    ofsmean << _nSamples[i] << " "<<  _avgM[i] << std::endl;
    ofsvar << _nSamples[i] << " "<< _avgV[i] << std::endl;
  }
}

// Main routine. 
// 
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
	}
	else if (_atype=="err")
	{
		LogLogLinearFit(_nSamples, _MSE, _convRate, _YIntError);
	}
}


/// Main routine using progressive mean and variance computations

void Analyzer::onlineAnalysis()
{
  _avgM.resize(_nSamples.size()) ;
  _avgV.resize(_nSamples.size()) ;
  
  for (int i=0; i<_nSamples.size(); i++)
  {
    _avgM[i]=0;
    _avgV[i]=0;
  }
  
  for (int i=0; i<_nSamples.size(); i++)
  {
    const int n(_nSamples[i]) ;
    vector<double> ms(_nReps,0) ;
    
    double mean = 0.0, variance = 0.0;
    for (int trial=1; trial <= _nReps; trial++)
    {
      vector<Point2D> S;
      _sampler->MTSample(S, n) ;
      
      vector<double> res ;
      _integrand->MultipointEval(res, S) ;
      double integralVal = progressive::MCEstimator(res);
      
      progressive::mean_progressive(mean, integralVal, trial);
      progressive::variance_progressive(variance, mean, integralVal, trial);
    }
    _avgM[i] = mean;
    _avgV[i] = variance;
  }
  
  if (_atype=="var")
  {
    LogLogLinearFit(_nSamples, _avgV, _convRate, _YIntError);
    _YIntError = exp(_YIntError);
  }
}
