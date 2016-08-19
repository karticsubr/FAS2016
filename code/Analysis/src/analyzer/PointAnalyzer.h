//
//  PointAnalyzer.h
//  Pointset
//
//  Created by Gurpreet Singh Bagga on 14/08/16.
//
//

#ifndef Pointset_PointAnalyzer_h
#define Pointset_PointAnalyzer_h

#include <Analyzer.h>
#include <sampler.h>

class PointAnalyzer : public Analyzer{

public:
    virtual void RunAnalysis(string& prefix);
    virtual void WriteFile(string& filename) const;
    virtual Analyzer* createAnalyzer(Sampler *s, const vector<string>& AnalyzerParams, const vector<string>& IntegString);

private:
    PointAnalyzer() { AnalyzerType = "pts" ;}
    PointAnalyzer(Sampler *s, const vector<string>& AnalyzerParams, const vector<string>& IntegString);

    static const string ShearStr; // --shear
    double _shear;

    friend class AnalyzerPrototype;

};



#endif
