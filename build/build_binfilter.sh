g++ -Wall -pedantic -I~/Downloads/jpeg-9b -c -o bin/binfilter.o binfilter.cpp
g++ -ljpeg -g -o bin/binfilter bin/binfilter.o -lm