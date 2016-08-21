#include <VarianceAnalyzer.h>
#include <cmdlnparser.h>

#include <integrand.h>

#include <iomanip>
#include <iostream>

///////////////////////////////////////////////
// VarianceAnalyzer class
// requires (pointers to) sampler and integrand objects
// and the portion of the commandline meant for the VarianceAnalyzer
//
// It uses the sampler and the integrand to estimate the estimator's
// convergence rate and the y-intercept of the convergence plot (log-log)
// The data is not in log-log, you can use ListLogLogPlot from mathematica
///////////////////////////////////////////////

// expected format for the analysis section is
// -A --nsamps n1 n2 n3 n4 ... --nreps r --atype a
//
// n1 n2 n3 n4 are integers eg. 10 100 500 1000 to be used as sample counts
// r is an integer specifying the number of n1-sample estimates to be averaged for computing
//    error at n1 (equal to number of n2-sample estimates to be averaged for error at n2, etc.)
// a is a string that can be "var" and is used to output the variance at n1, n2 ...
//
//

///
/// \brief VarianceAnalyzer::createAnalyzer
/// \param s
/// \param AnalyzerParams
/// \param IntegString
/// \return
///

Analyzer* VarianceAnalyzer::createAnalyzer(Sampler *s, const vector<string> &AnalyzerParams, const vector<std::string> &IntegString){
    return new VarianceAnalyzer(s, AnalyzerParams, IntegString);
}

VarianceAnalyzer::~VarianceAnalyzer(){
    //delete [] _integrand;
}

VarianceAnalyzer::VarianceAnalyzer(Sampler* s, const vector<string>& AnalyzerParams, const vector<std::string> &IntegString) {

    AnalyzerType = "Variance";
    _sampler = s;
    CLParser::FindMultiArgs<int>(-1, _nSamples, AnalyzerParams, NSampStr) ;
    _nTrials = CLParser::FindArgument<int>(AnalyzerParams, nTrialsStr) ;

    // create integrand object from the -I section of command line
    // implemented as a virtual constructor
    // treat this as a call to the new operator, and delete the object integrand responsibly
    _integrand = IntegrandPrototype::Generate(IntegString) ;
}

namespace progressive {

  inline double MCEstimator(const vector<double>& v)
  {
    double m(0);
    for(int i(0); i<v.size(); i++) m+=v[i] ;
    return m/v.size() ;
  }

  inline double compute_mean(double &mean, const double &integral, int trial)
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

  inline double compute_variance(double &variance, const double &mean, const double &integralVal, int trial)
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
}

void VarianceAnalyzer::RunAnalysis(string& prefix){

    //########################################################################################################
    ///
    /// We output three files:
    /// prefix-xxx-matlab.txt contains variance data written horizontally to plot directly from matlab
    /// prefix-mean.txt contains the mean value for each N as reference
    /// prefix-var.txt contains variance data vertically to plot in GNU or Mathematica
    ///
    std::stringstream ss;

    ss.str(std::string());
    ss << prefix << "-variance-matlab.txt";
    std::ofstream ofs(ss.str().c_str(), std::ofstream::app) ;

    ss.str(std::string());
    ss << prefix << "-mean.txt";
    std::ofstream ofsmean(ss.str().c_str(), std::ofstream::app) ;

    ss.str(std::string());
    ss << prefix << "-variance.txt";
    std::ofstream ofsvar(ss.str().c_str(), std::ofstream::app) ;

    ofs << std::fixed << std::setprecision(15);
    ofsmean << std::fixed << std::setprecision(15);
    ofsvar << std::fixed << std::setprecision(15);
    //########################################################################################################

    _avgM.resize(_nSamples.size()) ;
     _avgV.resize(_nSamples.size()) ;

     for (int i=0; i<_nSamples.size(); i++)
     {
       _avgM[i]=0;
       _avgV[i]=0;
     }

     for (int i=0; i<_nSamples.size(); i++){
       const int n(_nSamples[i]) ;

       double mean = 0.0, variance = 0.0;
       for (int trial=1; trial <= _nTrials; trial++)
       {
           fprintf(stderr, "\r trial/N:  %d / %d", trial, n);
           _pts.resize(0);
           _sampler->MTSample(_pts, n) ;

           vector<double> res;
           _integrand->MultipointEval(res, _pts, _sampler->GetType()) ;
           double integralVal = progressive::MCEstimator(res);

           progressive::compute_mean(mean, integralVal, trial);
           progressive::compute_variance(variance, mean, integralVal, trial);
       }

       _avgM[i] = mean;
       _avgV[i] = variance;

       ofsmean << n << " "<< _avgM[i] << std::endl;
       ofsvar << n << " "<< _avgV[i] << std::endl;
     }

     std::copy(_nSamples.begin(), _nSamples.end(), std::ostream_iterator<int>(ofs, " "));

     std::copy(_avgV.begin(), _avgV.end(), std::ostream_iterator<double>(ofs, " "));

     ofs << endl ;

     std::cerr << std::endl;
     ofs.close();
     ofsmean.close();
     ofsvar.close();
}

