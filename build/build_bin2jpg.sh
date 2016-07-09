g++ -Wall -pedantic -I~/Downloads/jpeg-9b -c -o bin/bin2jpg.o bin2jpg.cpp
g++ -ljpeg -g -o bin/bin2jpg bin/bin2jpg.o -lm