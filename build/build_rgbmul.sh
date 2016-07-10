g++ -Wall -pedantic -I~/Downloads/jpeg-9b -c -o bin/rgbmul.o rgbmul.cpp
g++ -ljpeg -g -o bin/rgbmul bin/rgbmul.o -lm