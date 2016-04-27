I Visualization code compilation (QT required)
Q.1 How to compile ?
Go to the directory code/
Execute the following commands:
mkdir build-visualization/
cd build-visualization/
qmake ./../code-visualization/
make
Now if you want your results to be in the folder code/results-visualization/
cd ./../
./visualizer Or
./visualizer.app/Contents/MacOS/visualizer
You will get a full description of possible arguments to pass through.

Q.2 Where are the results for the visualzation ? All results are in folder code/results-visualization/