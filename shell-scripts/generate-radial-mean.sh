#!/bin/sh

#  run-cft.sh
#  
#
#  Created by Gurprit Singh on 5/02/16.
#

./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/powerspectrum-regular-n4096/2016-04-29/datafiles/radial-mean-homogenize-regular-n4096-000001.txt --n 4096 --s regular --mode radial-mean

./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/powerspectrum-dartthrowing-n4096/2016-04-29/datafiles/radial-mean-homogenize-dartthrowing-n4091-000020.txt --n 4096 --s dartthrowing --mode radial-mean

./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/powerspectrum-multijitter-n1024/2016-04-29/datafiles/radial-mean-homogenize-multijitter-n1024-001000.txt --n 1024 --s multijitter --mode radial-mean

SAMPLER=(halton hammerslay)

for((ns=0;ns<2;ns++))
do
./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/powerspectrum-${SAMPLER[ns]}-n4096/2016-04-29/datafiles/radial-mean-homogenize-${SAMPLER[ns]}-n4096-001000.txt --n 4096 --s ${SAMPLER[ns]} --mode radial-mean
done


SAMPLER2=( jitter  random)

for((ns=0;ns<2;ns++))
do
./build-visualization/visualizer.app/Contents/MacOS/visualizer --file results/powerspectrum-${SAMPLER2[ns]}-n4096/2016-04-27/datafiles/radial-mean-homogenize-${SAMPLER2[ns]}-n4096-001000.txt --n 4096 --s ${SAMPLER2[ns]} --mode radial-mean
done


