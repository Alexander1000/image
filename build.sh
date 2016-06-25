g++ -Wall -pedantic -I/Users/amdankovtsev/Downloads/jpeg-9b -c -o binfilter.o binfilter.cpp
g++ -ljpeg -g -o binfilter binfilter.o -lm