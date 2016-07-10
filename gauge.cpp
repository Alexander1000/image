#include <iostream>
#include <math.h>
#include "bitmapcost.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    BitMapCost mapCost(argv[1]);
    mapCost.loadKFile(argv[2]);
    cout << mapCost.calc() << endl;
    return 0;
}
