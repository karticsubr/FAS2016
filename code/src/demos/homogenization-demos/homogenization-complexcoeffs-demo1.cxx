#include <iostream>
#include <fstream>
#include <iomanip>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <sys/stat.h>

#include "./../../core/utils.h"
#include "./../../samplers/sampler.h"
#include "./../../harmonic-analysis-euclid/fft.h"

namespace po = boost::program_options;

int main(int argc, char* argv[]){

    //############################################################
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("n", po::value<int>()->default_value(1024), "int: number of point samples")
        ("s", po::value<std::string>()->default_value("jitter"), "string: point sampling pattern")
        ("xfreq", po::value<double>()->default_value(25), "double: frequency x-coord")
        ("yfreq", po::value<double>()->default_value(20), "double: frequency y-coord")
        ("ntrials", po::value<int>()->default_value(1024), "int: number of trials");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") || vm.count("h") || argc == 1) {
        std::cout << desc << "\n";
        return 1;
    }
    //############################################################
    ///Arguments
    srand48(time(NULL));
    std::stringstream ss;
    int nsamples = vm["n"].as<int>();
    std::string samplingpattern = vm["s"].as<std::string>();
    double wx = vm["xfreq"].as<double>();
    double wy = vm["yfreq"].as<double>();
    int numTrials = vm["ntrials"].as<int>();
    //###############Creating Folders###################################

    ///Create folders to store results
    boost::filesystem::path source_dir_path( boost::filesystem::current_path() );

    std::string datafiles, images, graphs;
    ss.str(std::string());
    ss << source_dir_path.string() << "/results/";
    std::string resultFolder = ss.str();
    mkdir(resultFolder.c_str() ,0755);
    ss.str(std::string());
    ss << resultFolder << "homogenization-" <<  samplingpattern << "/";

    mkdir(ss.str().c_str(),0755);

    create_folders(ss.str(), datafiles, images, graphs);
    //std::cerr << datafiles << std::endl;
    //##########################################################

    //double domain[] = {-0.5,-0.5,0.5,0.5};
    double domain[] = {0,0,1,1};
    //##########################################################

    std::ofstream fcoeffs;
    ss.str(std::string());
    ss << datafiles << "homogenization-" << samplingpattern << "-n" << nsamples << "-wx" << wx << "-wy" << wy << ".txt";
    fcoeffs.open(ss.str().c_str());
    //##########################################################

    double normalization = 1.0 / (nsamples);
    for(int trial = 1; trial <= numTrials; trial++){
        std::vector<double> pointset;

        if(samplingpattern == "jitter"){
            pointset = jitter_samples2d(nsamples, domain);
        }
        else if(samplingpattern == "regular"){
            pointset = regular_samples2d(nsamples, domain);
        }
        else if(samplingpattern == "uniformjitter"){
            pointset = uniformjitter_samples2d(nsamples, domain);
        }
        else if(samplingpattern == "random"){
            pointset = random_samples2d(nsamples, domain);
        }
        else{
            std::cerr << "Requested sampling pattern not available !!!" << std::endl;
            return 1;
        }
        //##########################################################

        std::complex<double> coeff =  continous_fourier_transform(wx, wy, pointset);
        fcoeffs << std::fixed << std::setprecision(10) << coeff.real() * normalization <<" " << coeff.imag() * normalization << std::endl;
    }
    fcoeffs.close();

    return 0;
}

