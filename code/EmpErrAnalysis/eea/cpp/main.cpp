#include <iostream>
#include <string>
#include <randsampgsl.h>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <cmdlnparser.h>
#include <samples.h>
#include <integrand.h>

using namespace std ;

RandSampGSL gslSamps ;

#include <omp.h>
int target_thread_num = 4;
// omp_set_num_threads(target_thread_num);

int main(int argc, char* argv[])
{
	CLParser c(argc, argv) ;
	c.IdentifySections();
// 	cout << c.SamplerSection() << endl  << c.IntegSection() << endl  << c.AnalSection() << endl ;
	Sampler* s1 = SamplerPrototype::Generate(c.SamplerSection());
	
}


// int main(int argc, char* argv[])
// {
// //     unsigned int seed = time (NULL) * getpid();    
// //     srand(seed) ;
// //     srand48(seed) ;
// //        cout << drand48() << endl ;
//     ///////////////////////////////////
//     srand48(seedArg.getValue()) ;
//     
//     string ExMode (modeArg.getValue()) ;
// 
//     if (ExMode=="sample")
//     {
// 	try
// 	{
// 		// eg. ./eea -m sample -s Random -n 10 -f testout
// 		SamplerParams params(cmd) ;
// 		Sampler* s1 = SamplerPrototype::Generate(stypeArg.getValue(), numArg.getValue(), params);
// 		ofstream ofs(sampleFileArg.getValue().c_str());
// 		ofs << *s1 ;
// 		ofs.close() ;
// 	// 	s1->WriteEPSFile(sampleFileArg.getValue()+".eps");
// 	}
// 	catch( ArgParseException& ex)
// 	{
// 		cerr << "ERROR: " << ex.what() << endl ;		
// 		exit(1) ;
// 	}		
//     }
//        
//     if (ExMode=="primaryEstimError")
//     {
// 	// eg. ./eea -m primaryEstimError -s Random -i Gauss -n 100
// 	
// 	try
// 	{
// 		SamplerParams sparams (cmd);
// 		IntegrandParams iparams;
// 		
// 		Sampler* s1 = SamplerPrototype::Generate(stypeArg.getValue(), numArg.getValue(), sparams);
// 		Integrand* i1 = IntegrandPrototype::Generate(itypeArg.getValue(), iparams);
// 		vector<double> res ;
// 		i1->MultipointEval(res, s1->GetPoints()) ;
// 		
// 		float m(0); 
// 		for (int i(0); i<res.size(); i++)
// 			m += res[i] ;
// 		cout << "Error = " << (m-i1->ReferenceValue()) << endl ; 
// 	}
// 	catch(const exception& ex)
// 	{
// 		cerr << "ERROR: " << ex.what() << endl ;
// 		exit(1) ;
// 	}		
//     }
// 
//     if (ExMode=="secondaryEstimError")
// 	{
// 	// eg. ./eea -m secondaryEstimError -s Random -i QuadPix -n 100 -r 1000
// 	
// 	int n(numArg.getValue()), reps(numRepsArg.getValue()); 
// 	try
// 	{
// 		SamplerParams sparams (cmd);
// 		IntegrandParams iparams;
// 		
// 		Sampler* s1 = SamplerPrototype::Generate(stypeArg.getValue(), n, sparams);
// 		Integrand* i1 = IntegrandPrototype::Generate(itypeArg.getValue(), iparams);
// 		
// 		float mse(0) ;
// 		
// 		#pragma omp parallel for
// 		for (int r=0; r<reps; r++)
// 		{
// 			vector<Point2D> pts ;
// 			s1->MTSample(pts, n, sparams) ;
// 			vector<double> res ;
// 			i1->MultipointEval(res, pts) ;
// 			
// 			double m(0); 
// 			for (int i(0); i<res.size(); i++)
// 			{
// 				m += res[i] ;
// 			}
// 			m/=n ;
// 			mse += pow((m-i1->ReferenceValue()),2) ;
// 		}
// 		mse /= reps ;
// 		cout << "RMSE = " << sqrt(mse) << endl ; 
// 	}
// 	catch(const exception& ex)
// 	{
// 		cerr << "ERROR: " << ex.what() << endl ;
// 		exit(1) ;
// 	}		
//     }
// 
//     
//     return 1; 
// }
