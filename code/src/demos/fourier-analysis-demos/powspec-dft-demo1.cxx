#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <sys/stat.h>

#include "./../../io/write-image.h"
#include "./../../core/utils.h"
#include "./../../sampler-points/sampler-points.h"
#include "./../../fourier-analysis/fourier-analysis.h"

namespace po = boost::program_options;

int main(int argc, char* argv[]){

    //############################################################
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("n", po::value<int>(), "int: number of point samples")
        ("ntrials", po::value<int>()->default_value(1), "int: number of trials")
        ("step", po::value<int>()->default_value(1), "int: trial step size to output intermediate results")
        ("s", po::value<std::string>(), "string: point sampling pattern")
        ("feature", po::value<std::string>()->default_value("power"), "string: power || power-logscaled || power-scaled")
        ("accum", po::value<bool>()->default_value(true), "0 || 1")
        ("mode", po::value<std::string>()->default_value("homogenize"), "string: homogenize || nohomogenize");

    po::variables_map vm;
    //##########################################################
    ///If the given option does not match the allowed option throw exception
    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help") || vm.count("h") || argc == 1) {
            std::cout << desc << "\n";
            return 1;
        }
        po::notify(vm);
    }
    catch(po::error& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
        std::cerr << desc << std::endl;
        return 1;
    }
    //############################################################

    srand48(time(NULL));

    std::stringstream ss;
    int nsamples = vm["n"].as<int>();
    int numTrials = vm["ntrials"].as<int>();
    int stepSize = vm["step"].as<int>();
    std::string samplingpattern = vm["s"].as<std::string>();
    std::string mode = vm["mode"].as<std::string>();
    bool accumulate = vm["accum"].as<bool>();
    std::string feature = vm["feature"].as<std::string>();

    //###############Creating Folders###################################
    boost::filesystem::path source_dir_path( boost::filesystem::current_path() );

    std::string datafiles, images, graphs;
    ss.str(std::string());
    ss << source_dir_path.string() << "/results/";
    std::string resultFolder = ss.str();
    mkdir(resultFolder.c_str() ,0755);
    ss.str(std::string());
    ss << resultFolder << "powerspectrum-" <<  samplingpattern << "-n" << nsamples << "/";

    mkdir(ss.str().c_str(),0755);

    create_folders(ss.str(), datafiles, images, graphs);
    //##########################################################

    double bBoxMin = 0;
    double bBoxMax = 1;
    double domain[] = {bBoxMin, bBoxMin, bBoxMax, bBoxMax};

    double minDomainX = domain[0];
    double minDomainY = domain[1];
    double maxRangeX = domain[2] - domain[0];
    double maxRangeY = domain[3] - domain[1];

    //##########################################################
    int width = 512, height = 512;

    std::complex<double> *complexSpectrum = new std::complex<double>[width*height]();
    double* gridpoints = new double[width*height]();
    double* power = new double[width*height]();
    double* powerAccum = new double[width*height]();
    //##########################################################

    for(int trial = 1; trial <= numTrials; trial++){

        std::vector<double> pointset;

        if(samplingpattern == "jitter"){
            pointset = pointSampler2dd::jitter_samples(nsamples, domain);
        }
        else if(samplingpattern == "nrooks"){
            pointset = pointSampler2dd::nrooks_samples(nsamples, domain);
        }
        else if(samplingpattern == "regular"){
            pointset = pointSampler2dd::regular_samples(nsamples, domain);
        }
        else if(samplingpattern == "random"){
            pointset = pointSampler2dd::random_samples(nsamples, domain);
        }
        else if(samplingpattern == "uniformjitter"){
            pointset = pointSampler2dd::uniformjitter_samples(nsamples, domain);
        }
        else if(samplingpattern == "multijitter"){
            pointset = pointSampler2dd::multijitter_samples(nsamples, domain);
        }
        else if(samplingpattern == "dartthrowing"){
            pointset = pointSampler2dd::darthrowing_samples(nsamples, domain);
        }
        else if(samplingpattern == "bnot" || samplingpattern == "fpo" || samplingpattern == "step"){
            pointset = pointSampler2dd::bluenoise_samples(nsamples, domain, samplingpattern, source_dir_path.string());
        }
        else{
            std::cerr << "Requested sampling pattern not available !!!" << std::endl;
            return 1;
        }

        //##########################################################

        std::vector<double> finalsamples;
        if(mode == "homogenize")
            finalsamples = homogenize_pointsamples(pointset, domain, 2);
        else
            finalsamples = pointset;

        int N = finalsamples.size() * 0.5;
        //##########################################################


        fprintf(stderr,"\r N trial (%d %d)", N, trial);

        //##########################################################

        for(int k = 0; k < width*height; k++){
            gridpoints[k] = 0;
            power[k] = 0;
        }
        //##########################################################

        for(int i = 0; i < N; i++){
            int col = (pointset[2*i+0]-minDomainX) * (1.0 / maxRangeX) * width;
            int row = (pointset[2*i+1]-minDomainY) * (1.0 / maxRangeY) * height;
            gridpoints[row*width+col] = 1;
        }

        //##########################################################
        FT<double>::discrete_fourier_spectrum(complexSpectrum, gridpoints, width, height);
        FT<double>::power_fourier_spectrum(power, complexSpectrum, N, width, height);
        //perform_dft(power,gridpoints, N, width, height, "power");
        //##########################################################

        if(accumulate){
            for(int i = 0; i < width*height;i++)
                powerAccum[i] += power[i];
        }

        if(trial % stepSize == 0 || trial == 1){

            ss.str(std::string());
            ss << trial;
            std::string s1 = ss.str();
            paddedzerosN(s1, numTrials);
            //##########################################################

            for(int i = 0; i < width*height; i++)
                power[i] = powerAccum[i] / trial;

            //##########################################################

            FT<double>::swapQuadrants(power, width, height);

            //##########################################################
            ss.str(std::string());
            ss << images << "fourier-" << feature << "-" << mode << "-" << samplingpattern << "-n" << nsamples << "-" << s1 << ".png";
            write_exr_grey(ss.str(), power, width, height);

            ss.str(std::string());
            ss << images << "pointset-" << mode << "-" << samplingpattern << "-n" << nsamples << "-" << s1 << ".png";
            write_eps(ss.str(), finalsamples);
            //##########################################################

            ss.str(std::string());
            ss << datafiles << "radial-mean-" << mode << "-" << samplingpattern << "-n" << nsamples << "-" << s1 << ".txt";
            FT<double>::compute_radial_mean_powerspectrum(ss.str(), power, width, height, N);
        }
    }

    std::cerr << std::endl;

    delete [] complexSpectrum;
    delete [] gridpoints;
    delete [] power;

    return 0;
}


