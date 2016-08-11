import re
import sys

#input .pbrt file
filename = sys.argv[1]

#all the arguments containing crop window coordinates
args = map(float, sys.argv[2:])
#store crop window coordinates in simple variables
x1 = sys.argv[2]
x2 = sys.argv[3]
y1 = sys.argv[4]
y2 = sys.argv[5]

string = str('"float cropwindow" [' + x1 + ' ' + x2 + ' ' + y1 + ' ' + y2 + ']')

# Read in the file
filedata = None
with open(filename, 'r') as file :
    filedata = file.read()

# Replace the cropwindow with the input values in the .pbrt file
filedata = re.sub(r'"float cropwindow".+',string, filedata)

# Remove string with the filename in the .pbrt file
filedata = re.sub(r'"string filename".+','', filedata)

# Write the file out again
with open(filename, 'w') as file:
    file.write(filedata)