#include <iostream>
#include <jpeglib.h>
#include <vector>
#include <math.h>
#include "image.cpp"
//#include "pixel.cpp"
#include "filters/grayfilter.cpp"
#include "filters/zonefilter.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    Image image(argv[1]);

    cout << "Height: " << image.height << endl;
    cout << "Width: " << image.width << endl;

    /*GrayFilter filter(image.bitMap, image.width, image.height);
    int* grayMap = filter.filter();
    
    if (image.saveJpegBitmap("derevo2.jpg", grayMap, filter.width, filter.height) == 0) {
        cout << "Gray filter saved ..." << endl;
    }*/

    ZoneFilter smoothFilter(image.bitMap, image.width, image.height);

    if (image.saveJpegBitmap("derevo2.jpg", smoothFilter.filterSmooth(), image.width, image.height) == 0) {
        cout << "Smooth filter save ... " << endl;
    }

    ZoneFilter zonefilter(image.bitMap, image.width, image.height);

    if (image.saveJpegBitmap("derevo3.jpg", zonefilter.filter(), image.width, image.height) == 0) {
        cout << "Zone filter save ..." << endl;
    }

    return 0;
}
