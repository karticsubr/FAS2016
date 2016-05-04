
sig=$1 
strat=$2 
nsamps=$3
iter=$4
scene=$5
beta=$6  # for adding uniform jitter

ScenesPath='../Scenes/'
PBRTOut='../PBRTOut/'

boolval='false'

######
OutputEXR=$scene\_$strat\_$nsamps\_Samps\_iter_$iter\_$beta\_uj.exr
StratTag=\#$strat\#
sed  -e s/$StratTag/' '/   -e s/'_NSAMP_'/$nsamps/g -e s/'_FNAME_'/$OutputEXR/g  <$ScenesPath$scene > $ScenesPath'/tmp_'$scene\.pbrt

~/pbrt/pbrt-v3/pbrt $ScenesPath/tmp_$scene\.pbrt

mv $OutputEXR $PBRTOut$OutputEXR

#cp $scene.exr ../PBRTOut/$scene\_$strat\_$nsamps\_Samps\_$sig\_Sig_iter_$iter\_$beta\_uj.exr
# pfsin out.exr | pfsgamma --gamma 2.2 | pfsout ../data/pbrtout/$scene_$strat\_$nsamps\_Samps\_$sig\_Sig_iter_$iter.png
