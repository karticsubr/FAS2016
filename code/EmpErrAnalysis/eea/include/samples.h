
#ifndef __SAMPLESH_ 
#define __SAMPLESH_ 

#include <ostream>
#include <string>
#include <vector>
#include <point2d.h>
#include <map>
#include <random>

using std::vector ;
using std::map ;
using std::string; 
using std::ostream ;

class Sampler ;


///////////////////////////////////////////////
//	Basic implementation of the prototype
//  creational software design pattern, for 
//  creating the appropriate type of integrand object 
// given command line arguments for type and parameters.
///////////////////////////////////////////////
class SamplerPrototype
{
    public:
	SamplerPrototype() ;

	static Sampler* Generate(const vector<string>& SamplingSection) ;
    
    private:
	static map<string, Sampler*> exemplars;
};


///////////////////////////////////////////////
// Abstract base class for samplers
///////////////////////////////////////////////
class Sampler
{
    public:
	virtual Sampler* GenSampler(const vector<string>& SamplerParams) = 0 ;	
	virtual vector<Point2D>& Sample(int n) ; 			// simple sampling algo.
	virtual void MTSample(vector<Point2D>& pts, int n) const = 0;   // thread-safe version
	virtual string GetType() const {return SamplingType; } 	
	virtual vector<Point2D>& GetPoints() {return p;}	
	virtual ~Sampler() ;
	
	friend ostream& operator << (ostream& os, Sampler& s) ;
	
    protected:
	vector<Point2D> p ;
	string SamplingType ;
	mutable std::default_random_engine RGen;

};

// 				Subclasses of Sampler 

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Random
////////////////////////////////////////////////////////////////////////////////////////////////////////////
class randomSampler: public Sampler
{
    public:
	virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
	virtual void MTSample(vector<Point2D>& pts, int n) const ;	
	virtual ~randomSampler() {}
	
    private:
	randomSampler() {SamplingType = "Random" ;}
	randomSampler(const vector<string>& SamplerParams) ;
	friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Regular grid
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class gridSampler: public Sampler
{
    public:
	virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
	virtual void MTSample(vector<Point2D>& pts, int n) const ;	
	virtual ~gridSampler() {}
	
    private:
	gridSampler() {SamplingType = "Grid" ;}
	gridSampler(const vector<string>& SamplerParams) ;
	friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Jittered
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class jitteredSampler: public Sampler
{
    public:
	virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
	virtual void MTSample(vector<Point2D>& pts, int n) const ;	
	virtual ~jitteredSampler() {}
	
    private:
	jitteredSampler() {SamplingType = "Jittered" ;}
	jitteredSampler(const vector<string>& SamplerParams) ;
	friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Gaussian jitter
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class gjSampler: public Sampler
{
    public:
	virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
	virtual void MTSample(vector<Point2D>& pts, int n) const ;	
	virtual ~gjSampler() {}
		
    private:
	gjSampler(): _sigma(0.5) {SamplingType = "GJittered" ;}
	gjSampler(const vector<string>& SamplerParams) ;
	friend class SamplerPrototype;
	
	void ParseParameters(const vector<string>& SamplerParams) ;
	
	double _sigma ;
	static const string SigStr ; // = "--sigma" 
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Box-jitter
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class bjSampler: public Sampler
{
    public:
	virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
	virtual void MTSample(vector<Point2D>& pts, int n) const ;	
	virtual ~bjSampler() {}
	
    private:
	bjSampler(): _boxWidth(0.5) {SamplingType = "BJittered" ;}
	bjSampler(const vector<string>& SamplerParams) ;
	friend class SamplerPrototype;
	
	void ParseParameters(const vector<string>& SamplerParams) ;

	double _boxWidth ;
	static const string BWStr ; // = "--boxwidth" 	
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Poisson-disk (NOT IMPLEMENTED)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pdSampler: public Sampler
{
    public:
	virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
	virtual void MTSample(vector<Point2D>& pts, int n) const ;	
	virtual ~pdSampler() {}
	
    private:
	pdSampler() {SamplingType = "PDisk" ;}
	pdSampler(const vector<string>& SamplerParams) {} ;
	friend class SamplerPrototype;
};


#endif // __SAMPLESH_ 