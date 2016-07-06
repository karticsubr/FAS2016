#include <iostream>
#include <cmdlnparser.h>
#include <samples.h>
#include <integrand.h>
#include <Analyzer.h>

using namespace std ;

//time ./build/eea -m secondaryEstimError -S --stype GJittered --sigma .5 -I --itype QuadPix --random -A --atype err --nsamps  1 --nreps 50 


int main(int argc, char* argv[])
{
	CLParser clarg(argc, argv) ;
	clarg.IdentifySections();
	
	Sampler* s1 = SamplerPrototype::Generate(clarg.SamplerSection());
	
	Integrand* i1 = IntegrandPrototype::Generate(clarg.IntegSection()) ;
	
	Analyzer a(s1, i1, clarg.AnalSection()) ;
	a.RunAnalysis();
}

