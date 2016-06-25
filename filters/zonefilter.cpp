class ZoneFilter : public Filter
{
    public:
        ZoneFilter() : Filter() {
            // do something ...
        }

        ZoneFilter(int* bitMap, int width, int height) : Filter(bitMap, width, height) {
            // do something ...
         }

        int* filter() {
            this->buffer = (int*) malloc(width * height * sizeof(int));
            memcpy(this->buffer, this->originalBitMap, width * height * sizeof(int));

            this->bitMap = (int*) malloc(width * height * sizeof(int));

            // плавно сглаживаем цвета
            this->smoothBitMap(16);
            memcpy(this->buffer, this->preBitMap, width * height * sizeof(int));
            free(this->preBitMap);

            this->smoothBitMap(32);
            memcpy(this->buffer, this->preBitMap, width * height * sizeof(int));
            free(this->preBitMap);

            this->smoothBitMap(64);
            memcpy(this->buffer, this->preBitMap, width * height * sizeof(int));
            free(this->preBitMap);

            // todo: близкие цвета может сгладить дополнительно?

            // строим сигнатуру изображения
            this->buildSignature();

            // todo: добавить шумоподавление

            Pixel pixel;

            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    pixel.load(this->preBitMap[i * width + j]);
                    this->bitMap[i * width + j] = pixel.toInt32();
                }
            }

            return this->bitMap;
        }
    private:
        int* buffer;
        int* preBitMap;


        void smoothBitMap(int divider) {
            this->preBitMap = (int*) malloc(width * height * sizeof(int));
            Pixel pixel;
            Pixel newPixel;
            int half = divider / 2;
            int mod;

            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    pixel.load(this->buffer[i * this->width + j]);

                    mod = pixel.red % divider;

                    if (mod >= half) {
                        newPixel.red = pixel.red - mod + divider;
                    } else {
                        newPixel.red = pixel.red - mod;
                    }

                    mod = pixel.green % divider;

                    if (mod >= half) {
                        newPixel.green = pixel.green - mod + divider;
                    } else {
                        newPixel.green = pixel.green - mod;
                    }

                    mod = pixel.blue % divider;

                    if (mod >= half) {
                        newPixel.blue = pixel.blue - mod + divider;
                    } else {
                        newPixel.blue = pixel.blue - mod;
                    }

                    this->preBitMap[i * this->width + j] = newPixel.toInt32();
                }
            }
        }

        void buildSignature() {
            this->preBitMap = (int*) malloc(width * height * sizeof(int));
            int countDiff;
            Pixel curPixel;
            int curPixelInt;
            unsigned char gray;

            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    countDiff = 0;
                    curPixelInt = this->buffer[i * this->width + j];

                    if (i - 1 >= 0 && j - 1 >= 0 && curPixelInt != this->buffer[(i - 1) * this->width + j - 1]) {
                        ++countDiff;
                    }

                    if (i - 1 >= 0 && curPixelInt != this->buffer[(i - 1) * this->width + j]) {
                        ++countDiff;
                    }

                    if (i - 1 >= 0 && j + 1 <= this->width && curPixelInt != this->buffer[(i - 1) * this->width + j + 1]) {
                        ++countDiff;
                    }

                    if (j - 1 >= 0 && curPixelInt != this->buffer[i * this->width + j - 1]) {
                        ++countDiff;
                    }

                    if (j + 1 <= this->width && curPixelInt != this->buffer[i * this->width + j + 1]) {
                        ++countDiff;
                    }

                    if (i + 1 <= this->height && j - 1 >= 0 && curPixelInt != this->buffer[(i + 1) * this->width + j - 1]) {
                        ++countDiff;
                    }

                    if (i + 1 <= this->height && curPixelInt != this->buffer[(i + 1) * this->width + j]) {
                        ++countDiff;
                    }

                    if (i + 1 <= this->height && j + 1 <= this->width && curPixelInt != this->buffer[(i + 1) * this->width + j + 1]) {
                        ++countDiff;
                    }

                    gray = 0xFF >> countDiff;
                    curPixel.load(gray);
                    this->preBitMap[i * this->width + j] = curPixel.toInt32();
                }
            }
        }

        void analyseBitMap() {
            // do ...
        }
};
