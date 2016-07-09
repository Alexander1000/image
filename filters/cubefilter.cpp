using namespace std;

class CubeFilter : public Filter
{
    public:
        CubeFilter() : Filter() {
            // do something ...
        }

        CubeFilter(int* bitMap, int width, int height) : Filter(bitMap, width, height) {
            // do something ...
         }

        int* filter(int width, int height) {
            this->bitMap = (int*) malloc(this->width * this->height * sizeof(int));
            Pixel pixel;
            Pixel newPixel;

            for (int i = 0; i * height < this->height; ++i) {
                for (int j = 0; j * width < this->width; ++j) {
                    int r = 0, g = 0, b = 0;
                    int count = 0;

                    for (int ii = 0; ii < height; ++ii) {
                        for (int jj = 0; jj < width; ++jj) {
                            if (i * height + ii < this->height && j * width + jj < this->width) {
                                pixel.load(this->originalBitMap[(i * height + ii) * this->width + j * width + jj]);
                                r += pixel.red;
                                g += pixel.green;
                                b += pixel.blue;
                                ++count;
                            }
                        }
                    }

                    newPixel.load((UCHAR) (r / count), (UCHAR) (g / count), (UCHAR) (b / count));

                    for (int ii = 0; ii < height; ++ii) {
                        for (int jj = 0; jj < width; ++jj) {
                            if (i * height + ii < this->height && j * width + jj < this->width) {
                                this->bitMap[(i * height + ii) * this->width + j * width + jj] = newPixel.toInt32();
                            }
                        }
                    }
                }
            }

            return this->bitMap;
        }
};
