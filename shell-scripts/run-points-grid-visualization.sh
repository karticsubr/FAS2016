#!/bin/sh

#  run-points-grid-visualization.sh
#  
#
#  Created by Gurpreet Singh Bagga on 04/05/16.
#

DATE=2016-05-04

NSAMPLES=$1

SAMPLERS=(regular jitter multijitter nrooks uniformjitter random halton hammerslay)

for((ns=0;ns<9;ns++))
do
./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-${SAMPLERS[ns]}/${DATE}/datafiles/pointset-${SAMPLERS[ns]}-n${NSAMPLES:=16}.txt --n ${NSAMPLES:=16} --s ${SAMPLERS[ns]} --grid 1
done

#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-uniformjitter/${DATE}/datafiles/pointset-uniformjitter-n16-1.txt --s uniformjitter --n 16 --grid 1

#SAMPLERS2=(random halton hammerslay dartthrowing)
#NSAMPLES2=(16 64)
#
#for((ns=0;ns<4;ns++))
#do
#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-${SAMPLERS2[ns]}/${DATE}/datafiles/pointset-${SAMPLERS2[ns]}-n16.txt --n 16 --s ${SAMPLERS2[ns]} --grid 1
#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-${SAMPLERS2[ns]}/${DATE}/datafiles/pointset-${SAMPLERS2[ns]}-n64.txt --n 64 --s ${SAMPLERS2[ns]} --grid 1
#done



#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-random/2016-05-04/datafiles/pointset-random-n16.txt --s random --n 16
#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-random/2016-05-04/datafiles/pointset-random-n64.txt --s random --n 64
#
#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-hammerslay/2016-05-04/datafiles/pointset-hammerslay-n16.txt --s hammerslay --n 16
#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-hammerslay/2016-05-04/datafiles/pointset-hammerslay-n64.txt --s hammerslay --n 64
#
#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-halton/2016-05-04/datafiles/pointset-halton-n16.txt --s halton --n 16
#./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/pointset-halton/2016-05-04/datafiles/pointset-halton-n64.txt --s halton --n 64