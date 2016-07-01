#ifndef __CMDLNPARSER_H__
#define __CMDLNPARSER_H__

#include <string> 
#include <vector>


using std::string; 
using std::vector;


namespace CLArg
{
	const string SamplerSecStr("-S") ;
	const string IntegSecStr("-I") ;
	const string AnalSecStr("-A") ;
	const string GenSecStr("-G") ;
	
	const string SamplerType("--stype") ;
} ; 

class CLParser
{
    public:
	CLParser(int argc, char* argv[]) ;
	void IdentifySections() ;
	void ParseUnit() ;
	
	const vector<string>& SamplerSection() {return _samplerUnit;}
	const vector<string>& IntegSection() {return _integUnit;}
	const vector<string>& AnalSection() {return _analUnit;}

	static string  FindArgument(const vector<string>& args, const string& argStr) ;
	
    private:
	const int _argc ;
	char** _argv ;
	
	string _argvStr, _execMode ;
	vector<string> _samplerUnit, _integUnit, _analUnit ;
} ;


#endif // __CMDLNPARSER_H__