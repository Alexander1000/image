#include <iostream>
#include <jpeglib.h>
#include <vector>
#include <math.h>
#include "image.cpp"
#include "bitmap.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    BitMap bitMap(argv[1]);
    Image image;
    image.saveJpegBitmap(argv[2], bitMap.getBitMap(), bitMap.width, bitMap.height);
    return 0;
}
