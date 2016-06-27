#include <iostream>
#include <string>
#include <tclap/CmdLine.h>
#include <randsampgsl.h>
#include <fstream>
#include <ctime>
#include <unistd.h>

#include <samples.h>
#include <integrand.h>

using namespace TCLAP ;
using namespace std ;

RandSampGSL gslSamps ;
 
int main(int argc, char* argv[])
{
//     unsigned int seed = time (NULL) * getpid();    
//     srand(seed) ;
//     srand48(seed) ;
//        cout << drand48() << endl ;
     CmdLine cmd("Spectral profiles of sampling patterns", ' ', "1");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////// Parameters 
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ValueArg<string> modeArg("m","mode", "Execution mode",true, "", "string");
    cmd.add(modeArg) ;
    
    /////////////////////////////////// Mode: sample
    ValueArg<string> stypeArg("s","samplingtype", "Sampling type",false, "", "string");
    cmd.add(stypeArg) ;
    ValueArg<string> itypeArg("i","integrandtype", "Integrand type",false, "", "string");
    cmd.add(itypeArg) ;
    ValueArg<string> sampleFileArg("f","file", "Output filename",false, "samps", "string");
    cmd.add(sampleFileArg) ;
    ValueArg<int> numArg("n","num", "Number of primary samples",false, 16, "int");
    cmd.add(numArg) ;
    ValueArg<int> numRepsArg("r","reps", "Number of repetitions",false, 1000, "int");
    cmd.add(numRepsArg) ;
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
	// eg. ./eea -m sample -s Random -n 10 -f testout
	SamplerParams params ;
	params.sigma = sigArg.getValue() ; 
	Sampler* s1 = SamplerPrototype::Generate(stypeArg.getValue(), numArg.getValue(), params);
	ofstream ofs(sampleFileArg.getValue().c_str());
	ofs << *s1 ;
	ofs.close() ;
// 	s1->WriteEPSFile(sampleFileArg.getValue()+".eps");
    }
       
    if (ExMode=="primaryEstimError")
    {
	// eg. ./eea -m primaryEstimError -s Random -i Gauss -n 100
	
	try
	{
		SamplerParams sparams ;
		sparams.sigma = sigArg.getValue() ; 
		IntegrandParams iparams;
		
		Sampler* s1 = SamplerPrototype::Generate(stypeArg.getValue(), numArg.getValue(), sparams);
		Integrand* i1 = IntegrandPrototype::Generate(itypeArg.getValue(), iparams);
		vector<float> res ;
		i1->MultipointEval(res, s1->GetPoints()) ;
		
		float m(0); 
		for (int i(0); i<res.size(); i++)
			m += res[i] ;
		cout << "Error = " << (m-i1->ReferenceValue()) << endl ; 
	}
	catch(const exception& ex)
	{
		cerr << "ERROR: " << ex.what() << endl ;
		exit(1) ;
	}		
    }

    if (ExMode=="secondaryEstimError")
	{
	// eg. ./eea -m secondaryEstimError -s Random -i Gauss -n 100 -r 1000
	
	int n(numArg.getValue()), reps(numRepsArg.getValue()); 
	try
	{
		SamplerParams sparams ;
		sparams.sigma = sigArg.getValue() ; 
		IntegrandParams iparams;
		
		Sampler* s1 = SamplerPrototype::Generate(stypeArg.getValue(), n, sparams);
		Integrand* i1 = IntegrandPrototype::Generate(itypeArg.getValue(), iparams);
		
		float mse(0) ;
		#pragma omp parallel for
		for (int r=0; r<reps; r++)
		{
			vector<float> res ;
			i1->MultipointEval(res, s1->GetPoints()) ;
			
			float m(0); 
			for (int i(0); i<res.size(); i++)
				m += res[i] ;
			m/=n ;
			mse += pow((m-i1->ReferenceValue()),2) ;
		}
		mse /= reps ;
		cout << "RMSE = " << sqrt(mse) << endl ; 
	}
	catch(const exception& ex)
	{
		cerr << "ERROR: " << ex.what() << endl ;
		exit(1) ;
	}		
    }

    return 1; 
}
