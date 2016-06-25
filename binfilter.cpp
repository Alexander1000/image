#include <iostream>
#include <jpeglib.h>
#include "image.cpp"

using namespace std;

int main()
{
    Image image("derevo.jpg");

    cout << "Height: " << image.height << endl;
    cout << "Width: " << image.width << endl;

    if (image.saveJpeg("derevo2.jpg") == 0) {
        cout << "Image saved..." << endl;
    }

    return 0;
}
