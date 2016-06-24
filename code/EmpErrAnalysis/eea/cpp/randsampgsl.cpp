#include <randsampgsl.h>
#include <ctime>
#include <unistd.h>


RandSampGSL::RandSampGSL() 
{
    gsl_rng_env_setup();
    T = gsl_rng_default;
    gslrandr  = gsl_rng_alloc (T);
    unsigned int seed = time (NULL) * getpid();    
    gsl_rng_set (gslrandr, seed);      

}

 
RandSampGSL::~RandSampGSL() 
{
        gsl_rng_free (gslrandr);
}
