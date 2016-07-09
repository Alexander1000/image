#include <iostream>
#include <jpeglib.h>
#include "image.cpp"
#include "filters/cubefilter.cpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc == 5) {
        Image image(argv[1]);

        CubeFilter cubeFilter(image.bitMap, image.width, image.height);
        image.saveJpegBitmap(argv[2], cubeFilter.filter(atoi(argv[3]), atoi(argv[4])), image.width, image.height);
        return 0;
    } else {
        cout << "Help: [source_file.jpg] [destination_file.jpg] [width_pixel] [height_pixel]" << endl;
        return 1;
    }
}
