#include <iostream>
#include <math.h>
#include "bitmapcost.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    BitMapCost mapCost(argv[1]);
    mapCost.save(argv[2]);
    return 0;
}
