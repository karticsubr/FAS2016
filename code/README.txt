Source Code compilation
Q.1 How to compile ?

- Go to the directory code/

- Open CMakeLists.txt to check out the demonames

- Execute the following commands:

cmake -Ddemoname=on -Bbuild/ -Hcode/

make --directory=build/

./build/demoname

You will get a full description of possible arguments to pass through.

Q.2 Where are the results ? 

All results are in folder code/results/demoname/