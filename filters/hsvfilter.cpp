using namespace std;

class HsvMulFilter : public Filter
{
    public:
        HsvMulFilter() : Filter() {
            // do something ...
        }

        HsvMulFilter(int* bitMap, int width, int height) : Filter(bitMap, width, height) {
            // do something ...
         }

        int* filter(float h, float s, float v) {
            this->hsvMap = this->toHSV(this->originalBitMap, this->width, this->height);

            this->bitMap = this->toRGB(this->hsvMap, this->width, this->height);

            return this->bitMap;
        }

    private:
        int* bitMap;
        int** hsvMap;

        int* toRGB(int** hsvMap, int width, int height) {
            int* bitMap = (int*) malloc(height * width * sizeof(int));

            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    // Pixel pixel;
                    int* hsvPixel = hsvMap[i * width + j];
                    UCHAR v = (UCHAR) ceil(((float) hsvPixel[2] / 255) * 100);
                    int hue = (int) ceil(hsvPixel[0] / 60) % 6;
                    UCHAR vMin = (UCHAR) ceil((100 - hsvPixel[1]) * v); 
                    float a = (v - vMin) * (float) (hsvPixel[0] % 60) / 60;
                    UCHAR vInc = (UCHAR) ceil(vMin + a);
                    UCHAR vDec = (UCHAR) ceil((v - a));
                    bitMap[i * width + j] = this->hsvPixelToRGB(hue, v, vMin, vInc, vDec);
                }
            }

            return bitMap;
        }

        int hsvPixelToRGB(int hue, UCHAR v, UCHAR vMin, UCHAR vInc, UCHAR vDec) {
            Pixel pixel;

            switch (hue) {
                case 0: {
                    pixel.load(v, vInc, vMin);
                    break;
                }

                case 1: {
                    pixel.load(vDec, v, vMin);
                    break;
                }

                case 2: {
                    pixel.load(vMin, v, vInc);
                    break;
                }

                case 3: {
                    pixel.load(vMin, vDec, v);
                    break;
                }

                case 4: {
                    pixel.load(vInc, vMin, v);
                    break;
                }

                case 5: {
                    pixel.load(v, vMin, vDec);
                    break;
                }
            }

            return pixel.toInt32();
        }

        void freeHsvMap(int** hsvMap, int width, int height) {
            // todo foreach and free each vector [hue, saturation, value]
        }

        int** toHSV(int* bitMap, int width, int height) {
            int** hsvMap = (int**) malloc(height * width * sizeof(int*));

            Pixel pixel;

            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    // получаем компоненты RGB
                    pixel.load(bitMap[i * width + j]);
                    int* minMax = this->calcMaxMinRGB(&pixel);
                    int min = minMax[0], max = minMax[1];
                    free(minMax);

                    // выделяем память под компоненты HSV
                    int* hsvPixel = (int*) malloc(3 * sizeof(int));
                    // Hue (оттенок)
                    hsvPixel[0] = this->calcHue(min, max, &pixel);
                    // Saturation (насыщенность)
                    hsvPixel[1] = (int) ceil(this->calcSaturation(min, max) * 100);
                    // Value (Значение)
                    hsvPixel[2] = max;

                    // cout << "[" << i << ";" << j << "]: HSV (" << hsvPixel[0] << "; " << hsvPixel[1] << "; " << hsvPixel[2] << ")" << endl;

                    hsvMap[i * width + j] = hsvPixel;
                }
            }

            return hsvMap;
        }

        float calcSaturation(int min, int max) {
            if (max == 0) {
                return 0;
            }

            return 1 - ((float) min/max);
        }

        int calcHue(int min, int max, Pixel *pixel) {
            if (min == max) {
                return 0;
            }

            if (pixel->red == max && pixel->green >= pixel->blue) {
                return (int) ceil((float) 60 * (pixel->green - pixel->blue) / (max - min));
            }

            if (pixel->red == max && pixel->green < pixel->blue) {
                return (int) ceil((float) 60 * (pixel->green - pixel->blue) / (max - min) + 360);
            }

            if (pixel->green == max) {
                return (int) ceil((float) 60 * (pixel->blue - pixel->red) / (max - min) + 120);
            }

            if (pixel->blue == max) {
                return (int) ceil((float) 60 * (pixel->red - pixel->green) / (max - min) + 240);
            }

            return 0;
        }

        int* calcMaxMinRGB(Pixel* pixel) {
            int* result = (int*) malloc(2 * sizeof(int));

            int minColor = 255;
            int maxColor = 0;

            if (minColor > pixel->red) {
                minColor = pixel->red;
            }

            if (minColor > pixel->green) {
                minColor = pixel->green;
            }

            if (minColor > pixel->blue) {
                minColor = pixel->blue;
            }

            if (maxColor < pixel->red) {
                maxColor = pixel->red;
            }

            if (maxColor < pixel->green) {
                maxColor = pixel->green;
            }

            if (maxColor < pixel->blue) {
                maxColor = pixel->blue;
            }

            result[0] = minColor;
            result[1] = maxColor;

            return result;
        }
};
