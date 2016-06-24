#include <iostream>
#include <string>
#include <tclap/CmdLine.h>
#include <samples.h>
#include <randsampgsl.h>
#include <fstream>
#include <boost/progress.hpp>
#include <ctime>
#include <unistd.h>

using namespace TCLAP ;
using namespace std ;
 
RandSampGSL gslSamps ;
 
int main(int argc, char* argv[])
{
    unsigned int seed = time (NULL) * getpid();    
    srand(seed) ;
    srand48(seed) ;
//        cout << drand48() << endl ;
     CmdLine cmd("Spectral profiles of sampling patterns", ' ', "1");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////// Parameters 
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ValueArg<string> modeArg("m","mode", "Execution mode",true, "", "string");
    cmd.add(modeArg) ;
    
    /////////////////////////////////// Mode: sample
    ValueArg<string> stypeArg("t","type", "Sampling type",false, "", "string");
    cmd.add(stypeArg) ;
    ValueArg<string> sampleFileArg("f","file", "Output filename",false, "samps", "string");
    cmd.add(sampleFileArg) ;
    ValueArg<int> numArg("n","num", "Number of samples",false, 16, "int");
    cmd.add(numArg) ;
    ValueArg<unsigned int> seedArg("","seed", "Random seed",false, time(NULL), "unsigned int");
    cmd.add(seedArg) ;
    ValueArg<double> sigArg("","sigma", "Sigma for Gaussian jitter",false, 1, "double");
    cmd.add(sigArg) ;
    ///////////////////////////////////
    cmd.parse(argc, argv);
    
    srand48(seedArg.getValue()) ;
    
    string ExMode (modeArg.getValue()) ;

    if (ExMode=="sample")
    {
	// eg. ./eea -m sample -t Random -n 10 -f testout
      Params params ;
	params.sigma = sigArg.getValue() ; 
	samples* s1 = sampler::Generate(stypeArg.getValue(), numArg.getValue(), params);
	ofstream ofs(sampleFileArg.getValue().c_str());
	ofs << *s1 ;
	ofs.close() ;
// 	s1->WriteEPSFile(sampleFileArg.getValue()+".eps");
    }
       
    if (ExMode=="integrate")
    {
	// eg. ./eea -m integrate -t Random -n 100 -s 10 --integrand Gauss --params 1
      Params params ;
	params.sigma = sigArg.getValue() ; 
	samples* s1 = sampler::Generate(stypeArg.getValue(), numArg.getValue(), params);
	ofstream ofs(sampleFileArg.getValue().c_str());
	ofs << *s1 ;
	ofs.close() ;
    }

     return 1; 
}