import re
import sys
import math

#arguments
#filename.pbrt pbrtSampler nspp cropwindowCoordinates

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
    nsamples = int(math.sqrt(float(nsamples)))
    samplerstring = str('Sampler '+ '"' + sampler + '" ' + '"bool jitter" ["true"] ' + '"integer xsamples" [' + str(nsamples) +'] ' + '"integer ysamples" [' + str(nsamples) +']')
else:
    samplerstring = str('Sampler '+ '"' + sampler + '" ' + '"integer pixelsamples" [' + nsamples +']')


filedata = None
# Read in the file
with open(filename, 'r+') as file :
    filedata = file.read()

#cropWindowIsThere = re.search("float cropwindow", filedata);
#
#    if stringFileNameIsThere is None:
#        print stringFileNameIsThere;
#        file.seek(0,0);
#        file.write('"string filename" \n' + filedata);
#        filedata = file.read();
#        stringFileNameIsThere = re.search("string filename", filedata);
#        stringFileNameLocation = stringFileNameIsThere.start();
#    if cropWindowIsThere is None:
#        print stringFileNameIsThere;
#        file.seek(0,0);
#        file.write('"float cropwindow" \n' + filedata);
#        filedata = file.read();


# Replace the cropwindow with the input values in the .pbrt file
filedata = re.sub(r'"float cropwindow".+',cropwindowstring, filedata)

stringFileNameLocation = re.search("string filename", filedata).start();
#stringFileNameLocation = stringFileNameIsThere.start();
#stringFileNameLocation = re.search("string filename", filedata)
WorldBeginLocation = re.search("WorldBegin", filedata).start()

# Remove .exr or .png output-image filename from the .pbrt file.
# To Make sure not to remove .exr images used for Textures/EnvironmentMaps extra,
# only the first occurence of the "string filename" (.exr) declared before the "WorldBegin"
# is removed from the .pbrt file.
if stringFileNameLocation < WorldBeginLocation:
    print stringFileNameLocation;
    print WorldBeginLocation;
    print cropwindowstring

filedata = re.sub('"string filename" .+','"pbrt-eea.exr"', filedata, 1)

#if cropWindowIsThere is None:
#   filedata.write('"float cropwindow"\n')

# Replace the Sampler
filedata = re.sub(r'Sampler.+',samplerstring, filedata)

# Write the file out again
#with open(filename, 'w') as file:
#    file.write(filedata)
