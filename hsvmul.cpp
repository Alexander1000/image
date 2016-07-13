#include <iostream>
#include <jpeglib.h>
#include <math.h>
#include "image.cpp"
#include "filters/hsvfilter.cpp"

using namespace std;

int main(int argc, char* argv[]) {
    Image image(argv[1]);
    HsvMulFilter mulFilter(image.bitMap, image.width, image.height);
    image.saveJpegBitmap(argv[2], mulFilter.filter(atof(argv[3]), atof(argv[4]), atof(argv[5])), image.width, image.height);
    return 0;
}
