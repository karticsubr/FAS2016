#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <sys/stat.h>

#include "./../../io/write-image.h"
#include "./../../core/utils.h"
#include "./../../sampler-points/sampler-points.h"

namespace po = boost::program_options;

int main(int argc, char* argv[]){

    //############################################################
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("n", po::value<int>()->default_value(1024), "int: number of point samples")
        ("s", po::value<std::string>()->default_value("jitter"), "string: point sampling pattern")
        ("mode", po::value<std::string>()->default_value("nohomogenize"), "string: homogenize || nohomogenize");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") || vm.count("h") || argc == 1) {
        std::cout << desc << "\n";
        return 1;
    }

    boost::filesystem::path source_dir_path( boost::filesystem::current_path() );
//    std::cout << "Current path is : " << source_dir_path << std::endl;

    //############################################################
    srand48(time(NULL));
//    srand48(123456768);

    std::stringstream ss;
    int nsamples = vm["n"].as<int>();
    std::string samplingpattern = vm["s"].as<std::string>();
    std::string mode = vm["mode"].as<std::string>();

    //###############Creating Folders###################################
    std::string datafiles, images, graphs;
    ss.str(std::string());
    ss << source_dir_path.string() << "/results/";
    std::string resultFolder = ss.str();
    mkdir(resultFolder.c_str() ,0755);
    ss.str(std::string());
    ss << resultFolder << "pointset-" <<  samplingpattern << "/";

    mkdir(ss.str().c_str(),0755);

    create_folders(ss.str(), datafiles, images, graphs);
    //##########################################################

    double domain[] = {0,0,1,1};

    int width = 512, height = 512;
    double* testImage = new double[width*height]();
    //##########################################################

    std::vector<double> pointset;

    if(samplingpattern == "jitter"){
        pointset = pointSampler2dd::jitter_samples(nsamples, domain);
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
    else if(samplingpattern == "halton"){
        pointset = pointSampler2dd::halton_sequence_samples(nsamples, domain);
    }
    else if(samplingpattern == "hammerslay"){
        pointset = pointSampler2dd::hammersley_sequence_samples(nsamples, domain);
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
    //##########################################################

    std::ofstream fpoints;
    ss.str(std::string());

    if(mode == "homogenize")
        ss << datafiles << "pointset-" << mode <<"-" << samplingpattern << "-n" << nsamples << ".txt";
    else
        ss << datafiles << "pointset-" << samplingpattern << "-n" << nsamples << ".txt";

    fpoints.open(ss.str().c_str());

    //##########################################################

    int N = finalsamples.size() * 0.5;

    std::cerr << "# of samples: " << N << std::endl;

    for(int i = 0; i < N; i++){
        fpoints << finalsamples[2*i+0] << " "<< finalsamples[2*i+1] << std::endl;
        int row = finalsamples[2*i+0] * height;
        int col = finalsamples[2*i+1] * width;
        testImage[row*width+col] = 1;
    }

    fpoints.close();

    ss.str(std::string());
    ss << images << "pointset-" << mode << "-" << samplingpattern << "-n" << N << ".png";
    write_png_grey(ss.str(), testImage, width, height, true);

    return 0;
}

