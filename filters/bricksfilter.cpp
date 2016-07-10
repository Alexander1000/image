using namespace std;

class BricksFilter : public Filter
{
    public:
        BricksFilter() : Filter() {
            // do something ...
        }

        BricksFilter(int* bitMap, int width, int height) : Filter(bitMap, width, height) {
            this->blackHoll = true;
         }

        int* filter(int width, int height, int step) {
            this->bitMap = (int*) malloc(this->width * this->height * sizeof(int));
            // unsigned char a, r, g, b;
            Pixel pixel;
            Pixel newPixel;
            int red = 0, green = 0, blue = 0, count = 0;

            int bWidth = (int) ceil((float) this->width / width);
            int bHeight = (int) ceil((float) this->height / height);

            for (int i = 0; i < bHeight; ++i) {
                for (int j = 0; j < bWidth; ++j) {
                    red = 0, green = 0, blue = 0, count = 0;

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

                    if (this->blackHoll) {
                        for (int ii = 0; ii < height; ++ii) {
                            if (i * height + ii < this->height) {
                                this->bitMap[(i * height + ii) * this->width + j * width] = newPixel.toInt32();
                            }

                            if (i * height + ii < this->height) {
                                int offsetWidth = 0;

                                if ((j + 1) * width >= this->width) {
                                    offsetWidth = this->width - 1;
                                } else {
                                    offsetWidth = (j + 1) * width - 1;
                                }

                                this->bitMap[(i * height + ii) * this->width + offsetWidth] = newPixel.toInt32();
                            }
                        }

                        for (int ii = 0; ii < width; ++ii) {
                            if (j * width + ii < this->width) {
                                this->bitMap[i * height * this->width + j * width + ii] = newPixel.toInt32();
                            }

                            if (j * width + ii < this->width) {
                                int offsetHeight = 0;

                                if ((i + 1) * height >= this->height) {
                                    offsetHeight = this->height - 1;
                                } else {
                                    offsetHeight = (i + 1) * height - 1;
                                }

                                this->bitMap[offsetHeight * this->width + j * width + ii] = newPixel.toInt32();
                            }
                        }
                    } else {
                        // use colors
                        int lHeight = (i + 1) * height > this->height ? this->height - i * height : height;
                        int lWidth = (j + 1) * width > this->width ? this->width - j * width : width;
                        Pixel nnPixel;

                        for (int ii = 0; ii < lHeight; ++ii) {
                            for (int jj = 0; jj < lWidth; ++jj) {
                                if (ii == 0 || jj == 0 || ii == lHeight - 1 || jj == lWidth - 1) {
                                    this->bitMap[(i * height + ii) * this->width + j * width + jj] = newPixel.toInt32();
                                    continue;
                                }

                                if (ii <= step || jj <= step || ii >= lHeight - step || jj >= lWidth - step) {
                                    int test[4];
                                    test[0] = ii;
                                    test[1] = jj;
                                    test[2] = lHeight - step;
                                    test[3] = lWidth - step;
                                    int min = ii;

                                    for (int k = 0; k < 4; ++k) {
                                        if (test[k] < min) {
                                            min = test[k];
                                        }
                                    }

                                    float k = (float) min / step;

                                    pixel.load(this->originalBitMap[(i * height + ii) * this->width + j * width + jj]);
                                    float red, green, blue;

                                    if (pixel.red > newPixel.red) {
                                        red = pixel.red - (pixel.red - newPixel.red) * k;
                                    } else {
                                        red = pixel.red + (newPixel.red - pixel.red) * k;
                                    }

                                    if (pixel.green > newPixel.green) {
                                        green = pixel.green - (pixel.green - newPixel.green) * k;
                                    } else {
                                        green = pixel.green + (newPixel.green - pixel.green) * k;
                                    }

                                    if (pixel.blue > newPixel.blue) {
                                        blue = pixel.blue - (pixel.blue - newPixel.blue) * k;
                                    } else {
                                        blue = pixel.blue + (newPixel.blue - pixel.blue) * k;
                                    }

                                    nnPixel.load((UCHAR) ceil(red), (UCHAR) ceil(green), (UCHAR) ceil(blue));
                                    this->bitMap[(i * height + ii) * this->width + j * width + jj] = nnPixel.toInt32();
                                    continue;
                                }

                                pixel.load(this->originalBitMap[(i * height + ii) * this->width + j * width + jj]);
                                this->bitMap[(i * height + ii) * this->width + j * width + jj] = pixel.toInt32();
                            }
                        }
                    }
                }
            }

            return this->bitMap;
        }

        void setBlackHoll(bool blackHoll) {
            this->blackHoll = blackHoll;
        }

    private:
        bool blackHoll;
};
