
#ifndef __SAMPLESH_ 
#define __SAMPLESH_ 

#include <ostream>
#include <string>
#include <vector>
#include <point2d.h>
#include <map>

using std::vector ;
using std::map ;
using std::string; 
using std::ostream ;

class Sampler ;

struct SamplerParams
{
    double sigma ;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SamplerPrototype
{
    public:
	SamplerPrototype() ;

	static Sampler* Generate(const string& type, int n,  const SamplerParams& param) ;
	static map<string, Sampler*> exemplars;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Sampler
{
    public:
	virtual Sampler* GenSampler(int n, const SamplerParams& param) = 0 ;	
	virtual vector<Point2D>& Sample(int n, const SamplerParams& param) ;
	virtual void MTSample(vector<Point2D>& pts, int n, const SamplerParams& param) = 0;	
	virtual string GetType() const {return SamplingType; } 	
	virtual vector<Point2D>& GetPoints() {return p;}	
	virtual ~Sampler() ;
	
	friend ostream& operator << (ostream& os, Sampler& s) ;
	
    protected:
	vector<Point2D> p ;
	string SamplingType ;
};

// 				Subclasses of Sampler 

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Random
////////////////////////////////////////////////////////////////////////////////////////////////////////////
class randomSampler: public Sampler
{
    public:
	virtual Sampler* GenSampler(int n,  const SamplerParams& param)  ;
	virtual void MTSample(vector<Point2D>& pts, int n, const SamplerParams& param) ;	
	virtual ~randomSampler() {}
	
    private:
	randomSampler() {SamplingType = "Random" ;}
	randomSampler(int n,  const SamplerParams& param) ;
	friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Regular grid
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class gridSampler: public Sampler
{
    public:
	virtual Sampler* GenSampler(int n,  const SamplerParams& param)  ;
	virtual void MTSample(vector<Point2D>& pts, int n, const SamplerParams& param) ;	
	virtual ~gridSampler() {}
	
    private:
	gridSampler() {SamplingType = "Grid" ;}
	gridSampler(int n,  const SamplerParams& param) ;
	friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Jittered
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class jitteredSampler: public Sampler
{
    public:
	virtual Sampler* GenSampler(int n,  const SamplerParams& param)  ;
	virtual void MTSample(vector<Point2D>& pts, int n, const SamplerParams& param) ;	
	virtual ~jitteredSampler() {}
	
    private:
	jitteredSampler() {SamplingType = "Jittered" ;}
	jitteredSampler(int n,  const SamplerParams& param) ;
	friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Gaussian jitter
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class gjSampler: public Sampler
{
    public:
	virtual Sampler* GenSampler(int n,  const SamplerParams& param)  ;
	virtual void MTSample(vector<Point2D>& pts, int n, const SamplerParams& param) ;	
	virtual ~gjSampler() {}
	
    private:
	gjSampler() {SamplingType = "GJittered" ;}
	gjSampler(int n,  const SamplerParams& param) ;
	friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Poisson-disk
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class pdSampler: public Sampler
{
    public:
	virtual Sampler* GenSampler(int n,  const SamplerParams& param)  ;
	virtual void MTSample(vector<Point2D>& pts, int n, const SamplerParams& param) ;	
	virtual ~pdSampler() {}
	
    private:
	pdSampler() {SamplingType = "PDisk" ;}
	pdSampler(int n,  const SamplerParams& param) {} ;
	friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Box-jitter
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class bjSampler: public Sampler
{
    public:
	virtual Sampler* GenSampler(int n,  const SamplerParams& param)  ;
	virtual void MTSample(vector<Point2D>& pts, int n, const SamplerParams& param) ;	
	virtual ~bjSampler() {}
	
    private:
	bjSampler() {SamplingType = "BJittered" ;}
	bjSampler(int n,  const SamplerParams& param) ;
	friend class SamplerPrototype;
};


#endif // __SAMPLESH_ 