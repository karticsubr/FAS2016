import re
import sys
filename = sys.argv[1]
args = map(float, sys.argv[2:])
x1 = sys.argv[2]
x2 = sys.argv[3]
y1 = sys.argv[4]
y2 = sys.argv[5]

string = str('"float cropwindow" [' + x1 + ' ' + x2 + ' ' + y1 + ' ' + y2 + ']')

# Read in the file
filedata = None
with open(filename, 'r') as file :
    filedata = file.read()

# Replace the target string
filedata = re.sub(r'"float cropwindow".+',string, filedata)

# Write the file out again
with open(filename, 'w') as file:
    file.write(filedata)