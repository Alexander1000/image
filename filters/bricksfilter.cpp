using namespace std;

class BricksFilter : public Filter
{
    public:
        BricksFilter() : Filter() {
            // do something ...
        }

        BricksFilter(int* bitMap, int width, int height) : Filter(bitMap, width, height) {
            // do something ...
         }

        int* filter(int width, int height, int step) {
            this->bitMap = (int*) malloc(this->width * this->height * sizeof(int));
            // unsigned char a, r, g, b;
            Pixel pixel;
            Pixel newPixel;
            int red = 0, green = 0, blue = 0, count = 0;

            int bWidth = (int) ceil(this->width / width);
            int bHeight = (int) ceil(this->height / height);

            for (int i = 0; i < bHeight; ++i) {
                for (int j = 0; j < bWidth; ++j) {
                    for (int ii = 0; ii < height; ++ii) {
                        if (i * height + ii < this->height) {
                            pixel.load(this->originalBitMap[(i * height + ii) * this->width + j * width]);
                            red += pixel.red;
                            green += pixel.green;
                            blue += pixel.blue;
                            ++count;
                        }

                        if (i * height + ii < this->height) {
                            int offsetWidth = 0;

                            if ((j + 1) * width >= this->width) {
                                offsetWidth = this->width - 1;
                            } else {
                                offsetWidth = (j + 1) * width - 1;
                            }

                            pixel.load(this->originalBitMap[(i * height + ii) * this->width + offsetWidth]);
                            red += pixel.red;
                            green += pixel.green;
                            blue += pixel.blue;
                            ++count;
                        }
                    }

                    for (int ii = 0; ii < width; ++ii) {
                        if (j * width + ii < this->width) {
                            pixel.load(this->originalBitMap[i * height * this->width + j * width + ii]);
                            red += pixel.red;
                            green += pixel.green;
                            blue += pixel.blue;
                            ++count;
                        }

                        if (j * width + ii < this->width) {
                            int offsetHeight = 0;

                            if ((i + 1) * height >= this->height) {
                                offsetHeight = this->height - 1;
                            } else {
                                offsetHeight = (i + 1) * height - 1;
                            }

                            pixel.load(this->originalBitMap[offsetHeight * this->width + j * width + ii]);
                            red += pixel.red;
                            green += pixel.green;
                            blue += pixel.blue;
                            ++count;
                        }
                    }

                    newPixel.load((UCHAR) (red / count), (UCHAR) (green / count), (UCHAR) (blue / count));

                    for (int ii = 0; ii < height; ++ii) {
                        if (i * height + ii < this->height) {
                            this->bitMap[(i * height + ii) * this->width + j * width] = pixel.toInt32();
                        }

                        if (i * height + ii < this->height) {
                            int offsetWidth = 0;

                            if ((j + 1) * width >= this->width) {
                                offsetWidth = this->width - 1;
                            } else {
                                offsetWidth = (j + 1) * width - 1;
                            }

                            this->bitMap[(i * height + ii) * this->width + offsetWidth] = pixel.toInt32();
                        }
                    }

                    for (int ii = 0; ii < width; ++ii) {
                        if (j * width + ii < this->width) {
                            this->bitMap[i * height * this->width + j * width + ii] = pixel.toInt32();
                        }

                        if (j * width + ii < this->width) {
                            int offsetHeight = 0;

                            if ((i + 1) * height >= this->height) {
                                offsetHeight = this->height - 1;
                            } else {
                                offsetHeight = (i + 1) * height - 1;
                            }

                            this->bitMap[offsetHeight * this->width + j * width + ii] = pixel.toInt32();
                        }
                    }
                }
            }

            return this->bitMap;
        }
};
