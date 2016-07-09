g++ -Wall -pedantic -I/Users/amdankovtsev/Downloads/jpeg-9b -c -o bin/binfilter.o binfilter.cpp
g++ -ljpeg -g -o bin/binfilter bin/binfilter.o -lm

g++ -Wall -pedantic -I/Users/amdankovtsev/Downloads/jpeg-9b -c -o bin/bin2jpg.o bin2jpg.cpp
g++ -ljpeg -g -o bin/bin2jpg bin/bin2jpg.o -lm

g++ -Wall -pedantic -c -o bin/buildcostmap.o buildcostmap.cpp
g++ -g -o bin/buildcostmap bin/buildcostmap.o -lm

g++ -Wall -pedantic -c -o bin/gauge.o gauge.cpp
g++ -g -o bin/gauge bin/gauge.o -lm

g++ -Wall -pedantic -I/Users/amdankovtsev/Downloads/jpeg-9b -c -o bin/pixelator.o pixelator.cpp
g++ -ljpeg -g -o bin/pixelator bin/pixelator.o -lm