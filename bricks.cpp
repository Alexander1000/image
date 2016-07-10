#include <iostream>
#include <jpeglib.h>
#include <math.h>
#include "image.cpp"
#include "filters/bricksfilter.cpp"

using namespace std;

int main(int argc, char* argv[]) {
    Image image(argv[1]);
    BricksFilter brickFilter(image.bitMap, image.width, image.height);
    image.saveJpegBitmap(argv[2], brickFilter.filter(atoi(argv[3]), atoi(argv[4]), atoi(argv[5])), image.width, image.height);
    return 0;
}
