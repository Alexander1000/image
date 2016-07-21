g++ -Wall -pedantic -I~/Downloads/jpeg-9b -c -o bin/cost2jpg.o cost2jpg.cpp
g++ -ljpeg -g -o bin/cost2jpg bin/cost2jpg.o -lm