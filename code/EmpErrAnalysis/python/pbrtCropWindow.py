import re
import sys
import math

#arguments
#filename.pbrt pbrtSampler cropwindowCoordinates

#input .pbrt file
filename = sys.argv[1]
sampler = sys.argv[2]
nsamples = sys.argv[3]
#all the arguments containing crop window coordinates
args = map(float, sys.argv[4:])
#store crop window coordinates in simple variables
x1 = sys.argv[4]
x2 = sys.argv[5]
y1 = sys.argv[6]
y2 = sys.argv[7]

cropwindowstring = str('"float cropwindow" [' + x1 + ' ' + x2 + ' ' + y1 + ' ' + y2 + ']')



if sampler == 'stratified':
    nsamples = float(math.sqrt(float(nsamples)))
    print nsamples
    samplerstring = str('Sampler '+ '"' + sampler + '" ' + '"integer xsamples" [' + nsamples )#+'] ' + '"ysamples" [' + nsamples +']')
else:
    samplerstring = str('Sampler '+ '"' + sampler + '" ' + '"integer pixelsamples" [' + nsamples +']')

print samplerstring

# Read in the file
filedata = None
with open(filename, 'r') as file :
    filedata = file.read()

# Replace the cropwindow with the input values in the .pbrt file
filedata = re.sub(r'"float cropwindow".+',cropwindowstring, filedata)

# Remove string with the filename in the .pbrt file
filedata = re.sub(r'"string filename".+','', filedata)

# Replace the Sampler
filedata = re.sub(r'Sampler.+',samplerstring, filedata)

# Write the file out again
with open(filename, 'w') as file:
    file.write(filedata)