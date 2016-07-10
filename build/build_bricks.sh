g++ -Wall -pedantic -I~/Downloads/jpeg-9b -c -o bin/bricks.o bricks.cpp
g++ -ljpeg -g -o bin/bricks bin/bricks.o -lm