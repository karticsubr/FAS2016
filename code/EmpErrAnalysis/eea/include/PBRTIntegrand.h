//
//  PBRTIntegrand.h
//  eea
//
//  Created by Gurpreet Singh Bagga on 10/08/16.
//
//

#ifndef eea_PBRTIntegrand_h
#define eea_PBRTIntegrand_h


#include "integrand.h"

class PBRTIntegrand : public Integrand {
    
public:
    virtual Integrand* GenIntegrand(const vector<string>& IntegParams) ;
    virtual double operator () (const Point2D& p) const ;
    virtual ~PBRTIntegrand() ;
    
protected:
    PBRTIntegrand() { RefVal = 0.0; IntegrandType = "Pbrt" ; }
    PBRTIntegrand(const vector<string>& IntegParams);
    friend class IntegrandPrototype;
    
private:
    int _xpixel, _ypixel;
    std::string _pathexec, _pathscene;
    static const string PixelStr ; // = "--pixel"
    static const string PbrtExePathStr ; // = "--epath"
    static const string PbrtScenePathStr ; // = "-- spath"
};

#endif
