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

/*** Edited by Gurprit Singh ***/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <QImage>
#include <sstream>
#include <iomanip>

MainWindow::MainWindow(QString &outfile, std::vector<double> &data, std::string pattern, int N, int demoIndex,
                       double ptRadius, bool gridVisualize, double *domain, QWidget *parent) :  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  setupDemo(outfile, data, pattern, N, demoIndex, ptRadius, gridVisualize, domain);
}

void MainWindow::setupDemo(QString &outfile, std::vector<double> &data, std::string pattern, int N,
                           int demoIndex, double ptRadius, bool gridVisualize,
                           double* domain)
{
    switch (demoIndex){
    case 0: showPointsDemo(outfile, data, pattern, N, ui->customPlot, ptRadius, gridVisualize, domain); break;
    case 1: setupRadialSpectrumDemo(outfile, data, pattern, N, ui->customPlot, gridVisualize); break;
    }
    setWindowTitle("Plot: "+demoName);
    statusBar()->clearMessage();
    currentDemoIndex = demoIndex;
    ui->customPlot->replot();
}

void MainWindow::setupRadialSpectrumDemo(QString &outfile, std::vector<double> &data, std::string pattern,
                                         int N, QCustomPlot *customPlot, bool gridVisualize){

    setGeometry(50, 50, 1024, 512);
    demoName = "radial-spectrum";

    // generate some data:
    int dataSize = data.size() * 0.5;
    QVector<double> x(dataSize-1), y(dataSize-1); // initialize with entries 0..100
    std::cerr << "datasize:" << N << std::endl;
    for (int i=1; i<dataSize; ++i)
    {
      //x[i-1] = data[2*i]*(1/(sqrt(2*nSamples)));
        if(pattern == "ccvt")
            x[i-1] = data[2*i]*(1/(0.875*sqrt(N)));
        else if(pattern == "poissondisk" || pattern == "dartthrowing")
            x[i-1] = data[2*i]*(1/(0.908*sqrt(N)));
        else if(pattern == "jitter")
            x[i-1] = data[2*i]*(1/(sqrt(N)));
        else if(pattern == "multijitter")
            x[i-1] = data[2*i]*(1/(sqrt(N)));
        else if(pattern == "uniformjitter")
            x[i-1] = data[2*i]*(1/(sqrt(N)));
        else if(pattern == "nrooks")
            x[i-1] = data[2*i]*(1/(sqrt(N)));
        else if(pattern == "random")
            x[i-1] = data[2*i]*(1/(sqrt(N)));
        else if(pattern == "regular")
            x[i-1] = data[2*i]*(1/(sqrt(N)));
        else if(pattern == "halton")
            x[i-1] = data[2*i]*(1/(sqrt(N)));
        else if(pattern == "hammerslay")
            x[i-1] = data[2*i]*(1/(0.55*sqrt(N)));
        else{
            std::cerr << "Requested sampling pattern not available !!!" << std::endl;
            exit(-2);
        }
        y[i-1] = data[2*i+1];
    }

    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    customPlot->graph(0)->setPen(QPen(QColor(120, 120, 220), 1));
    customPlot->graph(0)->setBrush(QColor(20, 20, 220, 10));
    customPlot->graph(0)->setChannelFillGraph(customPlot->graph(0));

    customPlot->xAxis->setLabelFont(QFont( "Helvetica", 20));
    customPlot->yAxis->setLabelFont(QFont( "Helvetica", 20));


    // give the axes some labels:
    customPlot->xAxis->setLabel("Frequency");
    customPlot->yAxis->setLabel("Power");
    // set axes ranges, so we see all data:
    customPlot->rescaleAxes(true);

    customPlot->xAxis->setRange(0,4);
    //customPlot->xAxis->setRange(0,2.25);
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(1.0);
    customPlot->xAxis->setAutoSubTicks(false);
    customPlot->xAxis->grid()->setVisible(gridVisualize);

    customPlot->yAxis->setRange(0.0,2.0);
    customPlot->yAxis->setAutoTickStep(false);
    customPlot->yAxis->setTickStep(1.0);
    customPlot->yAxis->grid()->setVisible(gridVisualize);

    customPlot->setInteraction(QCP::iRangeDrag, true);
    customPlot->setInteraction(QCP::iRangeZoom, true);

    //std::cerr << outfile.toStdString() << std::endl;

    customPlot->savePdf(outfile,true, 512,256);
}

void MainWindow::showPointsDemo(QString &outfile, std::vector<double> &samples, std::string samplingpattern,
                                int N, QCustomPlot *customPlot, double ptRadius, bool gridVisualize, double *domain){
    setGeometry(50, 50, 768, 768);
    demoName = "Point set";

    double minDomainX = domain[0];
    double minDomainY = domain[1];
    double maxDomainX = domain[2];
    double maxDomainY = domain[3];
    double maxRangeX = domain[2] - domain[0];
    double maxRangeY = domain[3] - domain[1];

    // generate some data:
    int dataSize = samples.size() * 0.5;
    QVector<double> x(dataSize), y(dataSize); // initialize with entries 0..100

    customPlot->addGraph();
    customPlot->graph()->setPen(QColor(85,107,47, 255));
    customPlot->graph()->setLineStyle(QCPGraph::lsNone);
    customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, ptRadius));

    for (int i=0; i<dataSize; ++i)
    {
        x[i] = samples[2*i+0];
        y[i] = samples[2*i+1];
    }

    double strataLength = 1.0;
    double subStrataLength = 1.0;

//    if(samplingpattern == "jitter" || samplingpattern == "uniformjitter" || samplingpattern == "regular"){
//       strataLength = maxRangeX / sqrt(N);
//       subStrataLength = strataLength;
//    }
//    else if(samplingpattern == "multijitter" || samplingpattern == "nrooks"){
        strataLength = maxRangeX / sqrt(N);
        subStrataLength = maxRangeX / (N);
//    }

    std::cerr << strataLength << " " << subStrataLength << " "<< sqrt(N) << " " << N << std::endl;

    customPlot->graph()->setData(x, y);
    customPlot->graph()->rescaleAxes(true);

    ///Overlay Dots at the center of the disc visualization of points
    customPlot->addGraph();
    customPlot->graph(1)->setData(x, y);
    customPlot->graph(1)->rescaleAxes(true);
    customPlot->graph(1)->setPen(QColor(0,0,0, 255));
    customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 0.5));


    customPlot->xAxis->setRange(minDomainX,maxDomainX);

    QPen strataPen, subStrataPen;
    strataPen.setColor(Qt::black);
    subStrataPen.setColor(Qt::gray);
    subStrataPen.setStyle(Qt::DashLine);

    customPlot->xAxis->setRange(minDomainX,maxDomainX);
    customPlot->xAxis->grid()->setPen(strataPen);
    customPlot->xAxis->grid()->setSubGridPen(subStrataPen);
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setAutoSubTicks(false);
    customPlot->xAxis->setTicks(false);
    customPlot->xAxis->setTickStep(strataLength);
    customPlot->xAxis->setSubTickCount(sqrt(N)-1);
    //customPlot->xAxis->setSubTickLength(subStrataLength);
    customPlot->xAxis->grid()->setVisible(gridVisualize);
    customPlot->xAxis->grid()->setSubGridVisible(gridVisualize);

    customPlot->yAxis->setRange(minDomainY,maxDomainY);
    customPlot->yAxis->grid()->setPen(strataPen);
    customPlot->yAxis->grid()->setSubGridPen(subStrataPen);
    customPlot->yAxis->setAutoTickStep(false);
    customPlot->yAxis->setAutoSubTicks(false);
    customPlot->yAxis->setTicks(false);
    customPlot->yAxis->setTickStep(strataLength);
    customPlot->yAxis->setTickLength(strataLength);
    customPlot->yAxis->setSubTickCount(sqrt(N)-1);
    //customPlot->yAxis->setSubTickLength(subStrataLength);
    customPlot->yAxis->grid()->setVisible(gridVisualize);
    customPlot->yAxis->grid()->setSubGridVisible(gridVisualize);

    customPlot->xAxis2->setRange(minDomainY,maxDomainY);
    customPlot->yAxis2->setRange(minDomainY,maxDomainY);

    customPlot->xAxis2->setAutoTickStep(false);
    customPlot->xAxis2->setAutoSubTicks(false);
    customPlot->xAxis2->setTicks(false);

    customPlot->yAxis2->setAutoTickStep(false);
    customPlot->yAxis2->setAutoSubTicks(false);
    customPlot->yAxis2->setTicks(false);
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
//    customPlot->xAxis2->grid()->setVisible(true);
//    customPlot->yAxis2->grid()->setVisible(true);

//    customPlot->xAxis2->grid()->setPen(strataPen);
//    customPlot->xAxis2->grid()->setSubGridPen(subStrataPen);
//    customPlot->xAxis2->setTickLength(strataLength);
//    customPlot->xAxis2->setSubTickLength(subStrataLength);
//    customPlot->xAxis2->grid()->setVisible(gridVisualize);
//    customPlot->xAxis2->grid()->setSubGridVisible(true);
//    customPlot->xAxis2->setTicks(false);

//    customPlot->yAxis2->grid()->setPen(strataPen);
//    customPlot->yAxis2->grid()->setSubGridPen(subStrataPen);
//    customPlot->yAxis2->setTickLength(strataLength);
//    customPlot->yAxis2->setSubTickLength(subStrataLength);
//    customPlot->yAxis2->grid()->setVisible(gridVisualize);
//    customPlot->yAxis2->grid()->setSubGridVisible(true);
//    customPlot->yAxis2->setTicks(false);

    //    customPlot->xAxis->setAutoTickStep(false);
//    customPlot->xAxis->setTickStep(subStrataLength);

//    customPlot->xAxis->setVisible(gridVisualize);
//    customPlot->xAxis->grid()->setVisible(gridVisualize);

    //Make the top and right axes visible.
//    customPlot->xAxis2->setVisible(gridVisualize);
//    customPlot->xAxis2->setTicks(false);
    //customPlot->xAxis2->setRange(minDomainY,maxDomainY);
    //customPlot->xAxis2->setAutoTickStep(false);
    //customPlot->xAxis2->setTickStep(1.0);
    //customPlot->xAxis2->setTickLabels(false);


//    customPlot->yAxis->setAutoTickStep(false);
//    customPlot->yAxis->setTickStep(subStrataLength);
//    //customPlot->yAxis->setLabel("Real");
//    customPlot->yAxis->setVisible(gridVisualize);
//    customPlot->yAxis->grid()->setVisible(gridVisualize);
//    customPlot->yAxis->setTickLabels(false);
//    customPlot->yAxis->setTicks(false);
//    customPlot->yAxis->setSelectedLabelColor(QColor(1,1,1));

//    customPlot->yAxis2->setVisible(gridVisualize);
//    customPlot->yAxis2->setTicks(false);
////    customPlot->yAxis2->setRange(0,range);
////    customPlot->yAxis2->setAutoTickStep(false);
////    //customPlot->yAxis2->setTickStep(1.0);
////    customPlot->yAxis2->setTickLabels(false);


    customPlot->setInteraction(QCP::iRangeDrag, true);
    customPlot->setInteraction(QCP::iRangeZoom, true);

    customPlot->savePdf(outfile,true, 512,512);
}



