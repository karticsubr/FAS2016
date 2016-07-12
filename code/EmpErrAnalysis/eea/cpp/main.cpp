#include <iostream>
#include <cmdlnparser.h>
#include <samples.h>
#include <integrand.h>
#include <Analyzer.h>

using namespace std ;

// example commandline
//./build/eea -m secondaryEstimError -S --stype GJittered --sigma .5 -I --itype QuadPix --random -A --atype err --nsamps  1 --nreps 50 


int main(int argc, char* argv[])
{
	CLParser clarg(argc, argv) ;
	clarg.IdentifySections();
	
	// create sampler object from the -S section of command line
	// implemented as a virtual constructor
	// treat this as a call to the new operator, and delete the object s1 responsibly
	Sampler* s1 = SamplerPrototype::Generate(clarg.SamplerSection());
	
	// create integrand object from the -I section of command line
	// implemented as a virtual constructor
	// treat this as a call to the new operator, and delete the object i1 responsibly
	Integrand* i1 = IntegrandPrototype::Generate(clarg.IntegSection()) ;

	// create analyzer object from the -A section of command line
	Analyzer a(s1, i1, clarg.AnalSection()) ;

	
	a.RunAnalysis();

	
	string ofname(clarg.OutFile()) ;
	a.WriteResults(ofname);	
	
	delete i1; 
	delete s1;
}

