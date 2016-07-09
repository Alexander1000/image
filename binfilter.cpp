#include <iostream>
#include <jpeglib.h>
#include <vector>
#include <math.h>
#include "image.cpp"
//#include "pixel.cpp"
//#include "filters/grayfilter.cpp"
#include "filters/zonefilter.cpp"
#include "filters/binfilter.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    Image image(argv[1]);

    // filter for make black-white image
    ZoneFilter zonefilter(image.bitMap, image.width, image.height);
    int* bitMap = zonefilter.filter();

    // filter convert from black-white image to bin-format 
    BinFilter binFilter(bitMap, image.width, image.height);
    binFilter.save(argv[2]);

    return 0;
}
