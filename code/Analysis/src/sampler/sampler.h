#ifndef SAMPLER_H
#define SAMPLER_H

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

using namespace eea;

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
    virtual vector<Point2d>& Sample(int n) ; 			// simple sampling algo.
    virtual void MTSample(vector<Point2d>& pts, int n) const = 0;   // thread-safe version
    virtual string GetType() const {return SamplingType; }
    virtual vector<Point2d>& GetPoints() {return p;}
    virtual ~Sampler();

    Sampler(){bBoxMin = 0; bBoxMax = 1;}
    friend ostream& operator << (ostream& os, Sampler& s);

    std::vector<Point2d> toroidalWrapping(std::vector<Point2d> &p);

protected:
    vector<Point2d> p ;
    string SamplingType ;
    double bBoxMax, bBoxMin;
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
    virtual void MTSample(vector<Point2d>& pts, int n) const ;
    virtual ~randomSampler() {}

    private:
    randomSampler() {SamplingType = "random" ;}
    randomSampler(const vector<string>& SamplerParams) ;
    friend class SamplerPrototype;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 				Jittered
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class jitteredSampler: public Sampler
{
    public:
    virtual Sampler* GenSampler(const vector<string>& SamplerParams)  ;
    virtual void MTSample(vector<Point2d>& pts, int n) const ;
    virtual ~jitteredSampler() {}

    private:
    jitteredSampler() {SamplingType = "stratified" ;}
    jitteredSampler(const vector<string>& SamplerParams) ;
    friend class SamplerPrototype;
};


#endif // SAMPLER_H
