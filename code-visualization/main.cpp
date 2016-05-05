/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011, 2012, 2013, 2014 Emanuel Eichhammer               **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 07.04.14                                             **
**          Version: 1.2.1                                                **
****************************************************************************/

#include <QApplication>
#include <sstream>
#include <iostream>

#include <boost/program_options.hpp>

#include "mainwindow.h"
#include "core/io.h"
#include "core/utils.h"
#include <sys/stat.h>

namespace po = boost::program_options;

int main(int argc, char *argv[]){

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QApplication::setGraphicsSystem("raster");
#endif

    //############################################################
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("n", po::value<int>(), "int: number of point samples")
            ("s", po::value<std::string>(), "string: point sampling pattern")
            ("mode", po::value<std::string>()->default_value("points"), "string: points || radial-mean")
            ("file", po::value<std::string>(), "string: filename")
            ("dinf", po::value<double>()->default_value(0), "double: minimum (infimum) of the domain")
            ("dsupp", po::value<double>()->default_value(1), "double: maximum (supremum) of the domain")
            ("rad", po::value<double>()->default_value(8), "double: point radius")
            ("grid", po::value<bool>()->default_value(false), "bool: 0 or 1, to visualize the grid");

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

    QApplication a(argc, argv);
    std::vector<double> data;

    std::stringstream ss;
    int nsamples                = vm["n"].as<int>();
    std::string samplingpattern = vm["s"].as<std::string>();
    std::string mode            = vm["mode"].as<std::string>();
    std::string filename        = vm["file"].as<std::string>();
    double minDomain            = vm["dinf"].as<double>();
    double maxDomain            = vm["dsupp"].as<double>();
    double ptRadius             = vm["rad"].as<double>();
    bool gridVisualize          = vm["grid"].as<bool>();
    //############################################################

    //###############Creating Folders##############################

    std::string datafiles, images, graphs;
    std::string resultFolder = "./results-visualization/";
    mkdir(resultFolder.c_str() ,0755);
    ss << resultFolder << "/";
    create_folders(ss.str(), datafiles, images, graphs);
    std::cerr << datafiles << std::endl;
    //############################################################

    double domain[] = {minDomain, minDomain, maxDomain, maxDomain};
    read_pointsetnD(filename, data, 2);
    //############################################################

    ss.str(std::string());
    ss << images  << mode << "-" << samplingpattern << "-n" << nsamples << ".pdf";
    QString outputFileName = ss.str().c_str();
    //############################################################

    int demoIndex = -1;
    if(mode == "points")
        demoIndex = 0;
    else if(mode == "radial-mean")
        demoIndex = 1;
    else{
        std::cerr << "Requested mode is not available !!!" << std::endl;
        return 1;
    }
    //############################################################
//*/
//    QApplication a(argc, argv);
//    double domain[] = {-0.5,-0.5,0.5,0.5};
//    std::string filename = argv[2];
//    std::vector<double> data;
//    read_pointsetnD(filename, data, 2);
//    std::string samplingpattern = "jitter";
//    std::string ofname = "test.pdf";
//    QString outputFileName = ofname.c_str();
//    int demoIndex = 0;

//    std::cerr << data.size() << std::endl;

    MainWindow w(outputFileName, data, samplingpattern, nsamples, demoIndex, ptRadius, gridVisualize, domain);
    w.show();
    //############################################################

//    return 0;
    return a.exec();
}

