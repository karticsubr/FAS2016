#include <iostream>
#include <cmdlnparser.h>
#include <samples.h>
#include <integrand.h>
#include <Analyzer.h>

using namespace std ;

// example commandline
//-S --stype Jittered -I --itype Pbrt --epath /Users/gsingh/Work/bitbucket/pbrt-v3/build/pbrt --spath /Users/gsingh/Work/bitbucket/pbrt-v3-scenes/killeroos-moving/anim-killeroos.pbrt --img pbrt-eea.exr --pypath /Users/gsingh/Work/Projects/FAS2016/code/EmpErrAnalysis/python/pbrtCropWindow.py --crop 0.25 0.75 0.25 0.75 -A --atype var --nsamps  1 --nreps 1 -G --ofile test.txt
// ./build/eea -S --stype Jittered  
//    		-I --itype PWConstant --npts 100  --random 
// 		-A --atype var --nsamps  9 36 100 512  --nreps 1000 
// 		-G --ofile test.txt

int main(int argc, char* argv[])
{
	CLParser clarg(argc, argv);
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
  
//    string ofnameVar(clarg.OutFileVar()) ;
//    string ofnameMean(clarg.OutFileMean()) ;
//    a.WriteMeanVar(ofnameMean, ofnameVar) ;
	
	delete i1; 
	delete s1;
}

