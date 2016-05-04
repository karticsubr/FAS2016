#!/bin/bash

niter=1
pbrtScene='testmb_ball.scene'
beta=1

for iter in `seq 1 $niter`;
do
    for ns in 64
    do
	nsq=$(expr $ns \* $ns)
         ./RunPBRT.sh 0 LD $nsq $iter $pbrtScene
         ./RunPBRT.sh 0 RAND $nsq $iter $pbrtScene
         ./RunPBRT.sh 0 GRID $ns $iter $pbrtScene $beta
    done
done

