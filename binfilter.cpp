#include <iostream>
#include <jpeglib.h>
#include "image.cpp"
#include "pixel.cpp"
#include "filters/grayfilter.cpp"

using namespace std;

int main()
{
    Image image("derevo.jpg");

    cout << "Height: " << image.height << endl;
    cout << "Width: " << image.width << endl;

    if (image.saveJpeg("derevo2.jpg") == 0) {
        cout << "Image saved..." << endl;
    }

    GrayFilter filter(image.bitMap, image.width, image.height);
    
    if (image.saveJpegBitmap("derevo3.jpg", filter.filter(), filter.width, filter.height) == 0) {
        cout << "Gray filter saved ..." << endl;
    }

    return 0;
}
