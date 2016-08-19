#include <Analyzer.h>
#include <PointAnalyzer.h>
#include <cmdlnparser.h>
#include <sampler.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace eea;

const string PointAnalyzer::ShearStr = "--shear" ;

Analyzer* PointAnalyzer::createAnalyzer(Sampler *s, const vector<string> &AnalyzerParams, const vector<string> &IntegString){
    return new PointAnalyzer(s, AnalyzerParams, IntegString);
}

PointAnalyzer::PointAnalyzer(Sampler* s, const vector<string>& AnalyzerParams, const vector<string> &IntegString) {

    AnalyzerType = "pts";
    _sampler = s;
    CLParser::FindMultiArgs<int>(-1, _nSamples, AnalyzerParams, NSampStr) ;
    //_nSamples = CLParser::FindArgument<int>(AnalyzerParams, NSampStr) ;
    _shear = CLParser::FindArgument<double>(AnalyzerParams, ShearStr) ;
    _nTrials = CLParser::FindArgument<int>(AnalyzerParams, nTrialsStr) ;

//    std::cerr << _nSamples << " " << _shear << " "<< _nTrials<< std::endl;
}

void PointAnalyzer::WriteFile(string &filename) const{
    WriteEPS(filename);
}

void PointAnalyzer::RunAnalysis(string &prefix){

    for(int j = 0; j <_nSamples.size();j++){

        const int n(_nSamples[j]) ;
        _sampler->MTSample(_pts, n);

        ///Uncomment this function if your samples are going out of the domain
        //_sampler->toroidalWrapping(_pts);

        std::stringstream ss;
        ss << prefix << "-" << _sampler->GetType() << "-n" << n << ".txt";
        string filename = ss.str();
        WriteEPS(filename);
    }
    //double domain[] = {0,0,1,1};
    //toroidal_samples(_pts, domain);
}

