#ifndef VARIANCEANALYZER_H
#define VARIANCEANALYZER_H

#include "Analyzer.h"
#include "./../integrand/integrand.h"
#include <fstream>

class VarianceAnalyzer : public Analyzer{

public:
    virtual void RunAnalysis(string& prefix);
    virtual void WriteFile(string& filename) const;
    virtual Analyzer* createAnalyzer(Sampler *s, const vector<string>& AnalyzerParams, const vector<string>& IntegString);
    virtual ~VarianceAnalyzer();
private:
    VarianceAnalyzer() { AnalyzerType = "var" ;}
    VarianceAnalyzer(Sampler *s, const vector<string>& AnalyzerParams, const vector<string>& IntegString);

    static const string IntegrandStr; // --i
    string _integrandType;

    Integrand* _integrand;

    vector<double> _avgM, _avgV, _MSE ;

    friend class AnalyzerPrototype;
};

#endif // VARIANCEANALYZER_H
