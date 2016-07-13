g++ -Wall -pedantic -I~/Downloads/jpeg-9b -c -o bin/hsvmul.o hsvmul.cpp
g++ -ljpeg -g -o bin/hsvmul bin/hsvmul.o -lm