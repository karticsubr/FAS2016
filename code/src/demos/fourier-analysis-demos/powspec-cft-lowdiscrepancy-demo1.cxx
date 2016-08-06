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
            ("s", po::value<std::string>(), "string: point sampling pattern")
        ("u", po::value<int>(), "int: U coordinate component for low discrepancy sampler (u-v projection)")
        ("v", po::value<int>(), "int: V coordinate component for low discrepancy sampler (u-v projection)")
        ("ntrials", po::value<int>()->default_value(1), "int: number of trials")
        ("step", po::value<int>()->default_value(1), "int: trial step size to output intermediate results")
        ("feature", po::value<std::string>()->default_value("power"), "string: power || power-logscaled || power-scaled")
        ("accum", po::value<std::string>()->default_value("accumulate"), "noaccumulate || accumulate")
        ("mode", po::value<std::string>()->default_value("nohomogenize"), "string: homogenize || nohomogenize");


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
    int ucomponent = vm["u"].as<int>();
    int vcomponent = vm["v"].as<int>();
    int numTrials = vm["ntrials"].as<int>();
    int stepSize = vm["step"].as<int>();
    std::string samplingpattern = vm["s"].as<std::string>();
    std::string mode = vm["mode"].as<std::string>();
    std::string accumulate = vm["accum"].as<std::string>();
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
    double* power = new double[width*height]();
    double* powerAccum = new double[width*height]();
    //##########################################################

    for(int trial = 1; trial <= numTrials; trial++){

        std::vector<double> pointset;

        if(samplingpattern == "halton"){
            pointset = pointSampler2dd::halton_samples(nsamples, domain, ucomponent, vcomponent, false);
        }
        else if(samplingpattern == "hammerslay"){
            pointset = pointSampler2dd::hammerslay_samples(nsamples, domain, ucomponent, vcomponent, false);
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
            power[k] = 0;
        }

        //##########################################################
        FT<double>::continuous_fourier_spectrum(complexSpectrum, finalsamples, width, height,1.0);
        FT<double>::power_fourier_spectrum(power, complexSpectrum, N, width, height);
        //perform_dft(power,gridpoints, N, width, height, "power");
        //##########################################################

        for(int i = 0; i < width*height;i++)
            powerAccum[i] += power[i];

        if(trial % stepSize == 0 || trial == 1){

            ss.str(std::string());
            ss << trial;
            std::string s1 = ss.str();
            paddedzerosN(s1, numTrials);
            //##########################################################

            if(accumulate == "accumulate"){
                for(int i = 0; i < width*height; i++)
                    power[i] = powerAccum[i] / trial;
            }

            //##########################################################

//            FT<double>::swapQuadrants(power, width, height);

            //##########################################################
            ss.str(std::string());
            ss << images << "power-continuous-" << accumulate <<"-" << feature << "-" << mode << "-" << samplingpattern << "-u" <<ucomponent << "-v" << vcomponent << "-n" << nsamples << "-" << s1 << ".png";
            write_exr_grey(ss.str(), power, width, height);

            ss.str(std::string());
            ss << images << "pointset-" << mode << "-" << samplingpattern << "-u" <<ucomponent << "-v" << vcomponent << "-n" << nsamples << "-" << s1 << ".png";
            write_eps(ss.str(), finalsamples);
            //##########################################################

            ss.str(std::string());
            ss << datafiles << "radial-mean-continuous-"  << accumulate <<"-" << mode << "-" << samplingpattern << "-n" << nsamples << "-" << s1 << ".txt";
            FT<double>::compute_radial_mean_powerspectrum(ss.str(), power, width, height, N);
        }
    }

    std::cerr << std::endl;

    delete [] complexSpectrum;
    delete [] power;

    return 0;
}


