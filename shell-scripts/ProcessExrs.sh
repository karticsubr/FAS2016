# Example: ./ProcessExrs.sh '../data/pbrtout/testsha*' 50 ../data/pngs 

files=$1
ev=$2 
outpath=$3

for file in $files
do
  echo $file
  pfsin $file | pfsgamma -m $ev --gamma 2.2 | pfsout $file.ppm
  convert $file.ppm $file.png
  rm $file.ppm
  mv $file.png $outpath
done
