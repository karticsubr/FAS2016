#ifndef __VarianceAnalyzerH__
#define __VarianceAnalyzerH__

#include <iostream>
#include <vector>
#include <string>
#include <point2d.h>
#include <sampler.h>


using std::string ;
using std::cout ;
using std::endl ;
using std::vector ;

using namespace eea;

///////////////////////////////////////////////
// VarianceAnalyzer class
// requires (pointers to) sampler and integrand objects
// and the portion of the commandline meant for the VarianceAnalyzer
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

class Analyzer;

class AnalyzerPrototype{
public:
    AnalyzerPrototype() ;

    static Analyzer* Generate(Sampler *s, const vector<string>& AnalyzerString, const vector<string>& IntegString) ;

private:
    static map<string, Analyzer*> exemplars;

};

///////////////////////////////////////////////
// Abstract base class for analyzers
///////////////////////////////////////////////
class Analyzer{
public:
    virtual void RunAnalysis(string& prefix) = 0;
    //virtual void WriteFile(string& filename) const = 0;
    virtual Analyzer* createAnalyzer(Sampler *s, const vector<string>& SamplerParams, const vector<string>& IntegString) = 0 ;
    virtual string GetType() const {return AnalyzerType; }
    void create_folders(std::string homedir, std::string &data, std::string &images, std::string &graphs);
    virtual ~Analyzer();

protected:
    static const string nTrialsStr; // --nreps
    int _nTrials;

    static const string NSampStr; // --nsamps
    vector<int> _nSamples ; // secondary estimator sample sizes

    std::vector<Point2d> _pts;

    Sampler* _sampler ;

    string AnalyzerType;

    void WriteEPS(string& filename, double radius=2.0, double scale=512.0) const;
};

#endif //__AnalyzerH__
