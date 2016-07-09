g++ -Wall -pedantic -I~/Downloads/jpeg-9b -c -o bin/pixelator.o pixelator.cpp
g++ -ljpeg -g -o bin/pixelator bin/pixelator.o -lm