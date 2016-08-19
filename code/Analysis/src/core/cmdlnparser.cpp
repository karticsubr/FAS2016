#include <iostream>
#include <string>
#include <fstream>
#include "./../sampler/sampler.h"
#include "./../core/cmdlnparser.h"
#include <stdexcept>
#include <vector>
#include <sstream>

using namespace std ;
using namespace CLArg ;

namespace
{

    void strToVecStr (const string& str, vector<string>& outvec)
    {
        stringstream ss(str) ;
        while(ss)
        {
            string stemp ;
            ss >> stemp ;
// 			cout << stemp << endl ;
            outvec.push_back(stemp) ;
        }
    }
}

CLParser::CLParser(int argc, char* argv[]): _argc(argc), _argv(argv)
{
    stringstream ss;
    for (int i(0); i<_argc; i++) 	ss <<_argv[i] << " "  ;

    _argvStr = ss.str() ;

    ///
    /// Adding help message to begin with.
    ///
    if(argc > 1){
        std::string help = _argv[1];

        if(help == "-h" || help == "--h" || help == "-help" || help == "--help"){
            std::cerr << "Usage for different analyzers: " << std::endl;
            std::cerr << "Variance: ./build/main -S --stype stratified -A --atype var --nsamps  9 36 100 512  --nreps 1000 -G --ofile testing" << std::endl;
            std::cerr << "PointAnalyzer:./build/main -S --stype stratified -A --atype pts --shear 0 --n 1024 --t 1 -G --ofile pointset" << std::endl;
            std::cerr << "FourierAnalyzer:./build/main -S --stype stratified -A --atype fourier --n 4096 --t 10 --tstep 2 --wstep 1 -G --ofile powerspectrum" << std::endl;
            exit(1);
        }
    }
    else{
        std::cerr << "Usage for different analyzers: " << std::endl;
        std::cerr << "Variance: ./build/main -S --stype stratified -A --atype var --nsamps  9 36 100 512  --nreps 1000 -G --ofile testing" << std::endl;
        std::cerr << "PointAnalyzer:./build/main -S --stype stratified -A --atype pts --shear 0 --n 1024 --t 1 -G --ofile pointset" << std::endl;
        std::cerr << "FourierAnalyzer:./build/main -S --stype stratified -A --atype fourier --n 4096 --t 10 --tstep 2 --wstep 1 -G --ofile powerspectrum" << std::endl;
        exit(1);
    }
}

// Assumes that the command line contains strings in the order -S ... -A ... -G ...
// separates the sections based on the flags
void CLParser::IdentifySections()
{
    size_t Samp(_argvStr.find(SamplerSecStr)),
        Anal(_argvStr.find(AnalSecStr)), Gen(_argvStr.find(GenSecStr));

    size_t Integ = _argvStr.find(IntegSecStr);

    if(Integ != string::npos){

        if (Samp>=_argvStr.size())
                throw invalid_argument ("No Sampling section in command line.") ;
            if (Integ<Samp || Anal<Integ || Gen<Anal)
                throw invalid_argument ("Command line possibly missing section or is unordered: ensure -S ... -I ... -A ... -G ...") ;

            string SS = _argvStr.substr(Samp+3, Integ) ;
            strToVecStr (SS, _samplerUnit) ;

            _execMode = "Sample" ;

            if(Integ<_argvStr.size()-3)
            {
                string IS =  _argvStr.substr(Integ+3, Anal) ;
                strToVecStr (IS, _integUnit) ;
                _execMode = "AnalysePrimaryEstimator" ;
            }

            if(Anal<_argvStr.size()-3)
            {
                string AS = _argvStr.substr(Anal+3, Gen) ;
                strToVecStr (AS, _analUnit) ;
                _execMode = "AnalyzeSecondaryEstimator" ;
            }

            if(Gen<_argvStr.size()-3)
            {
                string GS = _argvStr.substr(Gen+3, _argvStr.size()) ;
                strToVecStr (GS, _genUnit) ;
            }
    }
    else{
        if (Samp>=_argvStr.size())
            throw invalid_argument ("No Sampling section in command line.") ;
        if (Anal<Samp || Gen<Anal)
            throw invalid_argument ("Command line possibly missing section or is unordered: ensure -S ... -A ... -G ...") ;

        string SS = _argvStr.substr(Samp+3, Anal) ;

        strToVecStr (SS, _samplerUnit) ;

        _execMode = "Sample" ;

        if(Anal<_argvStr.size()-3)
        {
            string IS =  _argvStr.substr(Anal+3, Gen) ;
            strToVecStr (IS, _analUnit) ;
            _execMode = "Analysis" ;
        }

        if(Gen<_argvStr.size()-3)
        {
            string GS = _argvStr.substr(Gen+3, _argvStr.size()) ;
            strToVecStr (GS, _genUnit) ;
        }
    }
}

// FindSwitch takes a string and tests if it is present in a given vector of strings
//            eg. FindSwitch(vec, "-A") returns true if an analysis section is found
bool CLParser::FindSwitch(const vector<string>& args, const string& argStr)
{
    for (int i(0); i<args.size(); i++)
        if (args[i]==argStr) return true;

    return false  ;
}


