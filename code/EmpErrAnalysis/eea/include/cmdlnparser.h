#ifndef __CMDLNPARSER_H__
#define __CMDLNPARSER_H__

#include <string> 
#include <vector>
#include <sstream>
#include <stdexcept>



using std::string; 
using std::vector;
using std::stringstream ;
using std::invalid_argument ;

namespace CLArg
{
	const string SamplerSecStr("-S") ;
	const string IntegSecStr("-I") ;
	const string AnalSecStr("-A") ;
	const string GenSecStr("-G") ;
	
	const string SamplerType("--stype") ;
	const string IntegrandType("--itype") ;
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

	static bool FindSwitch(const vector<string>& args, const string& switchStr) ;

	template<typename T>
	static T  FindArgument(const vector<string>& args, const string& argStr) ;

	template<typename T>
	static bool FindMultiArgs(int nargs, vector<T>& argOut, const vector<string>& args, const string& MultiArgsStr) ;

private:
	const int _argc ;
	char** _argv ;
	
	string _argvStr, _execMode ;
	vector<string> _samplerUnit, _integUnit, _analUnit ;
} ;


template<typename T>
T CLParser::FindArgument(const vector<string>& args, const string& argStr) 
{
	int found (args.size()) ;
	for (int i(0); i<args.size(); i++)
	{
		if (args[i]==argStr)
		{
			found = i ;
			break ;
		}
	}
	
	if (found<(args.size()-1))
	{
		T ret ;
		stringstream ss (args[found+1]) ;
		ss >> ret ;
		return ret ;
	}
	throw(invalid_argument("Did you forget to specify " + argStr + "?")) ;
	return T() ;
}

template<typename T>
bool CLParser::FindMultiArgs(int nargs, vector<T>& argOut, const vector<string>& args, const string& MultiArgsStr) 
{
	int found (args.size()) ;
	for (int i(0); i<args.size(); i++)
	{
		if (args[i]==MultiArgsStr)
		{
			found = i ;
			break ;
		}
	}
	
	if (nargs>0)
	{
		if (found<(args.size()-nargs))
		{
			argOut.resize(nargs) ;
			for (int i(0); i<nargs; i++) 
			{
				T ret ;
				stringstream ss (args[found+i+1]) ;
				ss >> ret ;
				argOut[i] = ret; 
			}
			return true ;
		}
		else if (found<args.size())
		{
			stringstream ss ;
			ss << nargs ;
			string nstr (ss.str()) ;
			
			throw(invalid_argument("Did you forget to specify enough parameters after " + MultiArgsStr + "?: Expecting " + nstr)) ;
		}
	}
	
	if (nargs<0) 
	{
		int i(found+1) ;
		bool readingArgs(true) ;
		while(readingArgs && i<args.size()) 
		{
			if (args[i][0] != '-')
			{
				T ret ;
				stringstream ss (args[i]) ;
				ss >> ret ;
				argOut.push_back(ret);
			}
			else 
				readingArgs=false; 
			i++ ;
		}
		if (argOut.size())
			return true; 
		else
			throw(invalid_argument("Did you forget to specify parameters after " + MultiArgsStr + "?: Expecting at least one since function is called with -ve #args")) ;
	}
	
	return  false ;
}


#endif // __CMDLNPARSER_H__