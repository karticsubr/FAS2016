#ifndef __RANDSAMP_GSL_
#define __RANDSAMP_GSL_
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>


class RandSampGSL
{
    public:
	RandSampGSL() ;
	double GaussianRandSample (double sigma) 
	{
	    return gsl_ran_gaussian(gslrandr, sigma) ;
	}
	~RandSampGSL() ;
    private:
	const gsl_rng_type* T;
	gsl_rng *gslrandr  ;
};

#endif // __RANDSAMP_GSL_
