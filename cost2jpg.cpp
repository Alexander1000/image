#include <iostream>
#include <jpeglib.h>
#include <vector>
#include <math.h>
#include "image.cpp"
#include "bitmapcost.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    BitMapCost bitMapCost;
    Image image;
    bitMapCost.loadKFile(argv[1]);
    bitMapCost.costToBitMap();
    image.saveJpegBitmap(argv[2], bitMapCost.getBitMapCost(), bitMapCost.kWidth, bitMapCost.kHeight);
    return 0;
}
