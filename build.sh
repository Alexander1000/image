g++ -Wall -pedantic -I/Users/amdankovtsev/Downloads/jpeg-9b -c -o binfilter.o binfilter.cpp
g++ -ljpeg -g -o binfilter binfilter.o -lm

g++ -Wall -pedantic -I/Users/amdankovtsev/Downloads/jpeg-9b -c -o bin2jpg.o bin2jpg.cpp
g++ -ljpeg -g -o bin2jpg bin2jpg.o -lm

g++ -Wall -pedantic -c -o buildcostmap.o buildcostmap.cpp
g++ -g -o buildcostmap buildcostmap.o -lm