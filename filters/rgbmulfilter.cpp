class RgbMulFilter : public Filter
{
    public:
        RgbMulFilter() : Filter() {
            // do something ...
        }

        RgbMulFilter(int* bitMap, int width, int height) : Filter(bitMap, width, height) {
            // do something ...
         }

        int* filter(float r, float g, float b) {
            this->bitMap = (int*) malloc(width * height * sizeof(int));
            // unsigned char a, r, g, b;
            Pixel pixel;
            Pixel newPixel;
            // int sum = 0;

            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    pixel.load(this->originalBitMap[i * width + j]);

                    float red = pixel.red * r;
                    float green = pixel.green * g;
                    float blue = pixel.blue * b;

                    newPixel.load(
                        (UCHAR) (red > 255 ? 255 : red),
                        (UCHAR) (green > 255 ? 255 : green),
                        (UCHAR) (blue > 255 ? 255 : blue)
                    );

                    this->bitMap[i * width + j] = newPixel.toInt32();
                }
            }

            return this->bitMap;
        }
};
