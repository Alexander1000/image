#include <iostream>
#include <math.h>
#include "pixel.cpp"
#include "bitmapcost.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    BitMapCost mapCost(argv[1]);
    mapCost.buildCost();
    mapCost.save(argv[2]);
    return 0;
}
