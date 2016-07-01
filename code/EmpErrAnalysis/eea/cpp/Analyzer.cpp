#include <Analyzer.h>



Analyzer::Analyzer(int argc, char* argv[]) 
{
	Parse(argc, argv) ;
}

void Analyzer::Parse(int argc, char* argv[]) 
{
	CmdLine cmd("Spectral profiles of sampling patterns --- Analysis mode ", ' ', "1");

	MultiArg<string> vsArg("S","sampler", "Sampling type(s) to be analyzed",true);
	cmd.add(vsArg) ;

	MultiArg<string> isArg("I","integrand", "Integrands to be used by analyzer",true);
	cmd.add(isArg) ;
	
	
	/*ValueArg<string> stypeArg("s","samplingtype", "Sampling type",false, "", "string");
	cmd.add(stypeArg) ;
	*/
	///////////////////////////////////
	cmd.parse(argc, argv);
    	
	const vector<string>& samps = vsArg.getValue() ;
	_ns = samps.size() ;
	
	for (int i(0); i<_ns; i++)
	{
	}

	const vector<string>& integs = isArg.getValue() ;
	_ni = integs.size() ;
	

	for (int i(0); i<_ni; i++)
	{
	}
	
}

void Analyzer::AssessAll() 
{
}

void Analyzer::AssessSelectedIntegrand(int integrandID) 
{
}

void Analyzer::AssessSelectedSampler(int samplerID) 
{
}
	
void Analyzer::WriteResults(const string& path) const 
{
}

