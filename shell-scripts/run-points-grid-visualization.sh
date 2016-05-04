#!/bin/sh

#  run-points-grid-visualization.sh
#  
#
#  Created by Gurpreet Singh Bagga on 04/05/16.
#

#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-uniformjitter/2016-04-29/datafiles/pointset-uniformjitter-n64-1.txt --s uniformjitter --n 64 --grid 1
#
#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-uniformjitter/2016-04-29/datafiles/pointset-uniformjitter-n64.txt --s uniformjitter --n 64 --grid 1
#
#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-regular/2016-04-29/datafiles/pointset-regular-n64.txt --n 64 --s regular --grid 1
#
#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-nrooks/2016-05-04/datafiles/pointset-nrooks-n64.txt --n 64 --s nrooks --grid 1
#
#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-jitter/2016-04-29/datafiles/pointset-jitter-n64.txt --n 64 --s jitter --grid 1
#
#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-multijitter/2016-04-29/datafiles/pointset-multijitter-n64.txt --n 64 --s multijitter --grid 1


DATE=2016-05-04

NSAMPLES=16

SAMPLERS=(regular jitter multijitter nrooks uniformjitter)

for((ns=0;ns<5;ns++))
do
#./build/pointset-demo1 --n ${NSAMPLES[ns]} --s ${SAMPLERS}
./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-${SAMPLERS[ns]}/${DATE}/datafiles/pointset-${SAMPLERS[ns]}-n${NSAMPLES}.txt --n ${NSAMPLES} --s ${SAMPLERS[ns]} --grid 1
done

#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-uniformjitter/${DATE}/datafiles/pointset-uniformjitter-n16-1.txt --s uniformjitter --n 16 --grid 1
