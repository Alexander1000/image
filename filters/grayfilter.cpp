class GrayFilter : public Filter
{
    public:
        GrayFilter() : Filter() {
            // do something ...
        }

        GrayFilter(int* bitMap, int width, int height) : Filter(bitMap, width, height) {
            // do something ...
         }

        int* filter() {
            this->bitMap = (int*) malloc(width * height * sizeof(int));
            // unsigned char a, r, g, b;
            Pixel pixel;
            Pixel newPixel;
            int sum = 0;

            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    sum = 0;
                    pixel.load(this->originalBitMap[i * width + j]);
                    sum = pixel.red + pixel.green + pixel.blue;
                    newPixel.load((UCHAR) (sum / 3));
                    this->bitMap[i * width + j] = newPixel.toInt32();
                }
            }

            return this->bitMap;
        }
};
