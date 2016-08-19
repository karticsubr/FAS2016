#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <sys/stat.h>
#include <Eigen/Dense>
#include "./../io/write-eps.h"
#include "./../core/utils.h"
#include "./../sampler-points/sampler-points.h"

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
    std::stringstream ss;
    int nsamples = vm["n"].as<int>();
    std::string samplingpattern = vm["s"].as<std::string>();
    std::string mode = vm["mode"].as<std::string>();

    //###############Creating Folders###################################
    std::string datafiles, images, graphs;
    ss.str(std::string());
    ss << source_dir_path.string() << "/results-analysis-shear/";
    std::string resultFolder = ss.str();
    mkdir(resultFolder.c_str() ,0755);
    ss.str(std::string());
    ss << resultFolder << "pointset-" <<  samplingpattern << "/";

    mkdir(ss.str().c_str(),0755);

    create_folders(ss.str(), datafiles, images, graphs);
    //##########################################################

    //double domain[] = {-0.5,-0.5,0.5,0.5};
    double domain[] = {0,0,1,1};
    //##########################################################

    std::vector<Point2d> pointset;

    if(samplingpattern == "jitter"){
        pointset = PointSampler2dd::jitter_samples(nsamples, domain);
    }
    else if(samplingpattern == "random"){
        pointset = PointSampler2dd::random_samples(nsamples, domain);
    }
    else{
        std::cerr << "Requested sampling pattern not available !!!" << std::endl;
        return 1;
    }
    //##########################################################

    int N = pointset.size();
    //##########################################################

    Eigen::Matrix2d shear;
    shear << 1, 0.2,
             0, 1;
    //##########################################################

    std::vector<Point2d> shearedSamples;
    for(int i = 0; i < N; i++){
        Vector2d v(pointset[i].x(), pointset[i].y());
        Vector2d tmp = shear * v;
        Point2d output(tmp.x(), tmp.y());
        shearedSamples.push_back(output);
    }

    PointSampler2dd::toroidal_samples(shearedSamples, domain);

    if(!PointSampler2dd::isBounded(shearedSamples, domain)){
        std::cerr << "samples going out of the domain !!!" << std::endl;
        exit(-2);
    }

    //##########################################################

    std::ofstream fpoints;
    ss.str(std::string());

    ss << datafiles << "pointset-" << mode <<"-" << samplingpattern << "-n" << nsamples << ".txt";

    fpoints.open(ss.str().c_str());

    //##########################################################

    std::cerr << "# of samples: " << N << std::endl;

    for(int i = 0; i < N; i++){
        fpoints << pointset[i].x() << " " << pointset[i].y() << std::endl;
    }

    fpoints.close();

    ss.str(std::string());
    ss << images << "pointset-sheared-" << mode << "-" << samplingpattern << "-n" << N << ".eps";
    write_eps_vector(ss.str(), shearedSamples);

    return 0;
}



