//
//  GaussianIntegrand.h
//  eea
//
//  Created by Gurpreet Singh Bagga on 23/07/16.
//
//

#ifndef eea_GaussianIntegrand_h
#define eea_GaussianIntegrand_h

#include "integrand.h"

class GaussianIntegrand : public Integrand {
  
public:
  virtual Integrand* GenIntegrand(const vector<string>& IntegParams) ;
  virtual double operator () (const Point2D& p) const ;
  virtual ~GaussianIntegrand() ;
  
protected:
  GaussianIntegrand() { RefVal = 0.0; IntegrandType = "gaussian" ; }
  GaussianIntegrand(const vector<string>& IntegParams);
  friend class IntegrandPrototype;
  
private:
  double _xsigma, _ysigma;
  Point2D _gaussianCenter;
  static const string SigmaStr ; // = "--sigma"
  static const string CenterStr ; // = "--center"
};


#endif
