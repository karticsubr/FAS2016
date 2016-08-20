
#include <tbb/tbb.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <tbb/task_scheduler_init.h>

#include <FourierAnalyzer.h>
#include <cmdlnparser.h>
#include <common.h>

#define USE_TBB
///
/// \brief FourierAnalyzer::trialStepStr
///

const string FourierAnalyzer::trialStepStr = "--tstep" ;
const string FourierAnalyzer::freqStepStr = "--wstep" ;

FourierAnalyzer::~FourierAnalyzer(){
    delete [] _powerSpectrum;
}

Analyzer* FourierAnalyzer::createAnalyzer(Sampler *s, const vector<string> &AnalyzerParams, const vector<std::string> &IntegString){
    return new FourierAnalyzer(s, AnalyzerParams, IntegString);
}

FourierAnalyzer::FourierAnalyzer(Sampler* s, const vector<string>& AnalyzerParams, const vector<std::string> &IntegString) : _trialStepOut(1), _frequencyStep(1.0) {

    AnalyzerType = "fourier";
    _sampler = s;
    CLParser::FindMultiArgs<int>(-1, _nSamples, AnalyzerParams, NSampStr) ;
    _nTrials = CLParser::FindArgument<int>(AnalyzerParams, nTrialsStr) ;
    _trialStepOut = CLParser::FindArgument<int>(AnalyzerParams, trialStepStr) ;
    _frequencyStep = CLParser::FindArgument<float>(AnalyzerParams, freqStepStr) ;

    _xRes = 512;
    _yRes = 512;
    _powerSpectrum = new float[_xRes * _yRes]();
    _complexSpectrum = new std::complex<float>[_xRes*_yRes]();

    //    std::cerr << _nSamples << " " << _shear << " "<< _nTrials<< std::endl;
}


template <typename T>
void FourierAnalyzer::continuous_fourier_spectrum(){

    int half_xRes = _xRes * 0.5;
    int half_yRes = _yRes * 0.5;
    int npoints = _pts.size();

 ///
 /// Comment out this code if don't want to use TBB Intel parallel threading
 /// and Uncooment the code at the end to use OpenMP for parallelization
 /// (OpenMP doesn't compile on some Mac OS systems)
 ///
    //tbb::tick_count t0 = tbb::tick_count::now();
    tbb::task_scheduler_init init(8);
    tbb::parallel_for(
                tbb::blocked_range2d<int>(0,_xRes, 16, 0, _yRes, 16),
                [=](const tbb::blocked_range2d<int>& imgblock ) {
        for( int row = imgblock.rows().begin(); row != imgblock.rows().end(); ++row ){
            for( int col = imgblock.cols().begin(); col != imgblock.cols().end(); ++col ) {
                T fx = 0.f, fy = 0.f;
                T wx = (col-half_xRes)*_frequencyStep;;
                T wy = (row-half_yRes)*_frequencyStep;;

                for (int i = 0; i < npoints; ++i) {
                    T exp = - (2*PI) * (wx * _pts[i].x + wy * _pts[i].y);
                    fx += cos(exp);
                    fy += sin(exp);
                }

                _complexSpectrum[row*_xRes+col].real(fx); ///real part
                _complexSpectrum[row*_xRes+col].imag(fy);  ///imaginary part
            }
        }
    }
    );

    ///
    /// Uncomment this code if you want to use OpenMP for parallelization
    ///
//#ifdef _OPENMP
//#pragma omp parallel
//#endif
//{
//#ifdef _OPENMP
//#pragma omp for schedule(static)
//#endif
//    for (int x = 0; x < _xRes; ++x) {
//        for (int y = 0; y < _yRes; ++y) {
//            float fx = 0.f, fy = 0.f;
//            float wx = x - (half_xRes)*_frequencyStep;
//            float wy = y - (half_yRes)*_frequencyStep;
//            for (int i = 0; i < npoints; ++i) {
//                float exp = -2*PI * (wx * _pts[i].x + wy * _pts[i].y);
//                fx += cosf(exp);
//                fy += sinf(exp);
//            }
//            _complexSpectrum[x + y*_xRes].real(fx); ///real part
//            _complexSpectrum[x + y*_xRes].imag(fy);  ///imaginary part
//        }
//    }
//}

}

//template void FourierAnalyzer::continuous_fourier_spectrum(double dstep);

template<typename T>
void FourierAnalyzer::power_fourier_spectrum(){

    for(int r = 0; r < _yRes; r++){
        for(int c = 0; c < _xRes; c++){
            T real = _complexSpectrum[(r*_xRes+c)].real();
            T imag = _complexSpectrum[(r*_xRes+c)].imag();

            T powerVal = (real*real + imag*imag) / (_pts.size());
            _powerSpectrum[(r*_xRes+c)] = powerVal;
        }
    }
}

void FourierAnalyzer::RunAnalysis(string& prefix){

    ///
    /// \brief powerAccum : Accumulate power over all the realizations (trials)
    ///
    float* powerAccum = new float[_xRes * _yRes]();

    for(int j = 0; j < _nSamples.size(); j++){

        const int n(_nSamples[j]) ;

        for(int trial = 1; trial <= _nTrials; trial++){

            _sampler->MTSample(_pts, n);

            for(int i=0; i<_xRes*_yRes; i++)
                _powerSpectrum[i] = 0.;

            continuous_fourier_spectrum<float>();
            power_fourier_spectrum<float>();

            for(int r=0; r < _yRes; r++){
                for(int c=0; c < _xRes; c++){
                    int index = r*_xRes+c;
                    powerAccum[index] += _powerSpectrum[index];
                }
            }

            ///
            /// Output final power in _powerSpectrum as an average over nTrials
            /// after each _trialStepOut steps.
            ///
            if(trial == 1 || trial % _trialStepOut == 0){
                for(int r=0; r < _yRes; r++){
                    for(int c=0; c < _xRes; c++){
                        int index = r*_xRes+c;
                        _powerSpectrum[index] = powerAccum[index] / float(trial);
                    }
                }

                ///
                /// Add index of each file with trailing zeros
                ///
                //##########################################################
                std::stringstream ss;
                ss.str(std::string());
                ss << trial;
                std::string s1 = ss.str();
                paddedzerosN(s1, _nTrials);
                //##########################################################
                ss.str(std::string());
                ss << prefix << "-" << s1 << ".exr";
                IO::WriteEXRgrey(ss.str(), _powerSpectrum, _xRes, _yRes);
            }
        }
    }
    delete [] powerAccum;
    powerAccum = 0;
}

