using namespace std;

class ZoneFilter : public Filter
{
    public:
        ZoneFilter() : Filter() {
            // do something ...
        }

        ZoneFilter(int* bitMap, int width, int height) : Filter(bitMap, width, height) {
            // do something ...
        }

        int* filterSmooth() {
            this->buffer = (int*) malloc(width * height * sizeof(int));
            memcpy(this->buffer, this->originalBitMap, width * height * sizeof(int));

            this->bitMap = (int*) malloc(width * height * sizeof(int));

            // плавно сглаживаем цвета
            this->smoothBitMap(16);
            this->smoothBitMap(32);
            this->smoothBitMap(64);

            this->cutPopularColor(3);

            this->smoothBitMap(64);

            //this->diffuseImage(64, 2);

            // строим сигнатуру изображения
            // this->buildSignature();

            Pixel pixel;

            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    pixel.load(this->buffer[i * width + j]);
                    this->bitMap[i * width + j] = pixel.toInt32();
                }
            }

            return this->bitMap;
        } 

        int* filter() {
            this->buffer = (int*) malloc(width * height * sizeof(int));
            memcpy(this->buffer, this->originalBitMap, width * height * sizeof(int));

            this->bitMap = (int*) malloc(width * height * sizeof(int));

            // плавно сглаживаем цвета
            this->smoothBitMap(16);
            this->smoothBitMap(32);
            this->smoothBitMap(64);

            // todo: дополнительно сгладить цвет?
            // this->diffuseImage(64, 3);

            // строим сигнатуру изображения
            this->buildSignature();

            // подавляем немного шума
            this->noiseClear(1, 64);
            this->noiseClear(2, 64);
            this->noiseClear(3, 48);
            this->noiseClear(4, 48);
            this->noiseClear(5, 32);
            this->noiseClear(6, 32);
            this->noiseClear(7, 16);
            this->noiseClear(8, 8);

            Pixel pixel;

            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    pixel.load(this->buffer[i * width + j]);
                    this->bitMap[i * width + j] = pixel.toInt32();
                }
            }

            return this->bitMap;
        }
    private:
        int* buffer;
        int* preBitMap;

        void cutPopularColor(int count) {
            this->preBitMap = (int*) malloc(width * height * sizeof(int));

            int red[256][2], green[256][2], blue[256][2];
            Pixel pixel;

            for (int i = 0; i < 256; ++i) {
                red[i][0] = 0;
                red[i][1] = i;
                green[i][0] = 0;
                green[i][1] = i;
                blue[i][0] = 0;
                blue[i][1] = i;
            }

            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    pixel.load(this->buffer[i * this->width + j]);
                    ++red[pixel.red][0];
                    ++green[pixel.green][0];
                    ++blue[pixel.blue][0];
                }
            }

            int temp[2];

            for (int i = 0; i < 256; ++i) {
                for (int j = 0; j < 255; ++j) {
                    if (red[j][0] < red[j + 1][0]) {
                        temp[0] = red[j][0];
                        temp[1] = red[j][1];
                        red[j][0] = red[j + 1][0];
                        red[j][1] = red[j + 1][1];
                        red[j + 1][0] = temp[0];
                        red[j + 1][1] = temp[1];
                    }

                    if (green[j][0] < green[j + 1][0]) {
                        temp[0] = green[j][0];
                        temp[1] = green[j][1];
                        green[j][0] = green[j + 1][0];
                        green[j][1] = green[j + 1][1];
                        green[j + 1][0] = temp[0];
                        green[j + 1][1] = temp[1];
                    }

                    if (blue[j][0] < blue[j + 1][0]) {
                        temp[0] = blue[j][0];
                        temp[1] = blue[j][1];
                        blue[j][0] = blue[j + 1][0];
                        blue[j][1] = blue[j + 1][1];
                        blue[j + 1][0] = temp[0];
                        blue[j + 1][1] = temp[1];
                    }
                }
            }

            Pixel newPixel(red[count][1], green[count][1], blue[count][1]);

            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    pixel.load(this->buffer[i * this->width + j]);
                    bool redExist = false, greenExist = false, blueExist = false;

                    for (int k = 0; k < count / 3; ++k) {
                        if (red[k][1] == pixel.red) {
                            redExist = true;
                        }

                        if (green[k][1] == pixel.green) {
                            greenExist = true;
                        }

                        if (blue[k][1] == pixel.blue) {
                            blueExist = true;
                        }
                    }

                    if (redExist && greenExist && blueExist) {
                        this->preBitMap[i * this->width + j] = newPixel.toInt32();
                    } else {
                        this->preBitMap[i * this->width + j] = pixel.toInt32();
                    }
                }
            }

            memcpy(this->buffer, this->preBitMap, width * height * sizeof(int));
            free(this->preBitMap);
        }

        void smoothBitMap(int divider) {
            this->preBitMap = (int*) malloc(width * height * sizeof(int));
            Pixel pixel;
            Pixel newPixel;
            int half = divider >> 1;
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

            memcpy(this->buffer, this->preBitMap, width * height * sizeof(int));
            free(this->preBitMap);
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

            memcpy(this->buffer, this->preBitMap, width * height * sizeof(int));
            free(this->preBitMap);
        }

        /**
         * diff - разница между 2-мя цветами
         * countDiffs - количество отличающихся цветов
         */
        void diffuseImage(int diff, int countDiffs) {
            this->preBitMap = (int*) malloc(width * height * sizeof(int));
            Pixel curPixel;
            Pixel pixel;
            Pixel diffPixel;
            int nDiffs;
            vector<int*> fills;
            int halfDiff = diff >> 1;

            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    curPixel.load(this->buffer[i * this->width + j]);

                    if (i - 1 >= 0) {
                        nDiffs = 0;
                        pixel.load(this->buffer[(i - 1) * this->width + j]);
                        diffPixel.load(pixel.toInt32());

                        if (abs(pixel.red - curPixel.red) <= diff && pixel.red != curPixel.red) {
                            ++nDiffs;
                            diffPixel.red = pixel.red > curPixel.red ? curPixel.red + halfDiff : pixel.red + halfDiff;
                        }

                        if (abs(pixel.green - curPixel.green) <= diff && pixel.green != curPixel.green) {
                            ++nDiffs;
                            diffPixel.green = pixel.green > curPixel.green ? curPixel.green + halfDiff : pixel.green + halfDiff;
                        }

                        if (abs(pixel.blue - curPixel.blue) <= diff && pixel.blue != curPixel.blue) {
                            ++nDiffs;
                            diffPixel.blue = pixel.blue > curPixel.blue ? curPixel.blue + halfDiff : pixel.blue + halfDiff;
                        }

                        if (countDiffs >= nDiffs && nDiffs > 0) {
                            int* params = (int*) malloc(5 * sizeof(int));
                            params[0] = i - 1;
                            params[1] = j;
                            params[2] = i;
                            params[3] = j;
                            params[4] = diffPixel.toInt32();
                            fills.push_back(params); 
                        }
                    }

                    if (j - 1 >= 0) {
                        nDiffs = 0;
                        pixel.load(this->buffer[i * this->width + j - 1]);
                        diffPixel.load(pixel.toInt32());

                        if (abs(pixel.red - curPixel.red) <= diff && pixel.red != curPixel.red) {
                            ++nDiffs;
                            diffPixel.red = pixel.red > curPixel.red ? curPixel.red + halfDiff : pixel.red + halfDiff;
                        }

                        if (abs(pixel.green - curPixel.green) <= diff && pixel.green != curPixel.green) {
                            ++nDiffs;
                            diffPixel.green = pixel.green > curPixel.green ? curPixel.green + halfDiff : pixel.green + halfDiff;
                        }

                        if (abs(pixel.blue - curPixel.blue) <= diff && pixel.blue != curPixel.blue) {
                            ++nDiffs;
                            diffPixel.blue = pixel.blue > curPixel.blue ? curPixel.blue + halfDiff : pixel.blue + halfDiff;
                        }

                        if (countDiffs >= nDiffs && nDiffs > 0) {
                            int* params = (int*) malloc(5 * sizeof(int));
                            params[0] = i;
                            params[1] = j - 1;
                            params[2] = i;
                            params[3] = j;
                            params[4] = diffPixel.toInt32();
                            fills.push_back(params); 
                        }
                    }

                    if (j + 1 < this->width) {
                        nDiffs = 0;
                        pixel.load(this->buffer[i * this->width + j + 1]);
                        diffPixel.load(pixel.toInt32());

                        if (abs(pixel.red - curPixel.red) <= diff && pixel.red != curPixel.red) {
                            ++nDiffs;
                            diffPixel.red = pixel.red > curPixel.red ? curPixel.red + halfDiff : pixel.red + halfDiff;
                        }

                        if (abs(pixel.green - curPixel.green) <= diff && pixel.green != curPixel.green) {
                            ++nDiffs;
                            diffPixel.green = pixel.green > curPixel.green ? curPixel.green + halfDiff : pixel.green + halfDiff;
                        }

                        if (abs(pixel.blue - curPixel.blue) <= diff && pixel.blue != curPixel.blue) {
                            ++nDiffs;
                            diffPixel.blue = pixel.blue > curPixel.blue ? curPixel.blue + halfDiff : pixel.blue + halfDiff;
                        }

                        if (countDiffs >= nDiffs && nDiffs > 0) {
                            int* params = (int*) malloc(5 * sizeof(int));
                            params[0] = i;
                            params[1] = j + 1;
                            params[2] = i;
                            params[3] = j;
                            params[4] = diffPixel.toInt32();
                            fills.push_back(params); 
                        }
                    }

                    if (i + 1 < this->height) {
                        nDiffs = 0;
                        pixel.load(this->buffer[(i + 1) * this->width + j]);
                        diffPixel.load(pixel.toInt32());

                        if (abs(pixel.red - curPixel.red) <= diff && pixel.red != curPixel.red) {
                            ++nDiffs;
                            diffPixel.red = pixel.red > curPixel.red ? curPixel.red + halfDiff : pixel.red + halfDiff;
                        }

                        if (abs(pixel.green - curPixel.green) <= diff && pixel.green != curPixel.green) {
                            ++nDiffs;
                            diffPixel.green = pixel.green > curPixel.green ? curPixel.green + halfDiff : pixel.green + halfDiff;
                        }

                        if (abs(pixel.blue - curPixel.blue) <= diff && pixel.blue != curPixel.blue) {
                            ++nDiffs;
                            diffPixel.blue = pixel.blue > curPixel.blue ? curPixel.blue + halfDiff : pixel.blue + halfDiff;
                        }

                        if (countDiffs >= nDiffs && nDiffs > 0) {
                            int* params = (int*) malloc(5 * sizeof(int));
                            params[0] = i + 1;
                            params[1] = j;
                            params[2] = i;
                            params[3] = j;
                            params[4] = diffPixel.toInt32();
                            fills.push_back(params); 
                        }
                    }
                }
            }

            memcpy(this->preBitMap, this->buffer, width * height * sizeof(int));

            //cout << fills.size() << endl;
            //int i = 0;
            for (int i = 0; i < fills.size(); ++i) {
                this->fillMap(fills[i][0], fills[i][1], fills[i][4]);
                this->fillMap(fills[i][2], fills[i][3], fills[i][4]);
            }

            memcpy(this->buffer, this->preBitMap, width * height * sizeof(int));
            free(this->preBitMap);
        }

        /**
         * Заливка
         * x - номер строчки
         * y - номер столбца
         */
        void fillMap(int x, int y, int color) {
            Pixel pixel(this->preBitMap[x * this->width + y]);
            int curColor = pixel.toInt32();

            if (curColor == color) {
                return;
            }

            bool* matrix = (bool*) malloc(this->width * this->height * sizeof(bool));
            memset(matrix, false, this->width * this->height * sizeof(bool));

            matrix[x * this->width + y] = true;

            this->scanFill(matrix, curColor, color, x, y);

            /*for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    if (matrix[i * this->width + j] == true) {
                        this->preBitMap[i * this->width + j] = color;
                    }
                }
            }*/

            free(matrix);
        }

        void scanFill(bool* matrix, int color, int fillColor, int fillX, int fillY) {
            bool test = false;
            int xMin = fillX, xMax = fillX + 1, yMin = fillY, yMax = fillY + 1;
            int iStart, iStop, jStart, jStop;

            while (true) {
                test = false;
                iStart = xMin;
                jStart = yMin;
                iStop = xMax;
                jStop = yMax;

                for (int i = iStart; i < iStop; ++i) {
                    for (int j = jStart; j < jStop; ++j) {
                        if (matrix[i * this->width + j] == true) {
                            if (i - 1 >= 0 && matrix[(i - 1) * this->width + j] == false && this->preBitMap[(i - 1) * this->width + j] == color) {
                                matrix[(i - 1) * this->width + j] = true;
                                this->preBitMap[(i - 1) * this->width + j] = fillColor;
                                test = true;

                                if (xMin > i - 1) {
                                    xMin = i - 1;
                                }
                            }

                            if (j - 1 >= 0 && matrix[i * this->width + j - 1] == false && this->preBitMap[i * this->width + j - 1] == color) {
                                matrix[i * this->width + j - 1] = true;
                                this->preBitMap[i * this->width + j - 1] = fillColor;
                                test = true;

                                if (yMin > j - 1) {
                                    yMin = j - 1;
                                }
                            }

                            if (j + 1 < this->width && matrix[i * this->width + j + 1] == false && this->preBitMap[i * this->width + j + 1] == color) {
                                matrix[i * this->width + j + 1] = true;
                                this->preBitMap[i * this->width + j + 1] = fillColor;
                                test = true;

                                if (yMax < j + 1) {
                                    yMax = j + 1;
                                }
                            }

                            if (i + 1 < this->height && matrix[(i + 1) * this->width + j] == false && this->preBitMap[(i + 1) * this->width + j] == color) {
                                matrix[(i + 1) * this->width + j] = true;
                                this->preBitMap[(i + 1) * this->width + j] = fillColor;
                                test = true;

                                if (xMax < i + 1) {
                                    xMax = i + 1;
                                }
                            }
                        }
                    }
                }

                if (test == false) {
                    break;
                }
            }            
        }

        /**
         * 8 (ячеек вокруг) х 8 (оттенков серого) = 64
         * kDot - оттенок серого зачищаемых точек 1..8
         * k - коэффициент подавления шума 1..64
         */
        void noiseClear(int kDot, int k) {
            this->preBitMap = (int*) malloc(width * height * sizeof(int));
            Pixel curPixel;
            int kCurDot;
            int kSumDots;
            Pixel pixel;

            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    kSumDots = 0;
                    curPixel.load(this->buffer[i * this->width + j]);
                    // kCurDot = 8 - ((curPixel.red + 1) >> 5);
                    kCurDot = this->getKGray(curPixel.red, 8);

                    if (kCurDot != kDot) {
                        this->preBitMap[i * this->width + j] = curPixel.toInt32();
                        continue;
                    }

                    if (i - 1 >= 0 && j - 1 >= 0) {
                        pixel.load(this->buffer[(i - 1) * this->width + j - 1]);
                        kSumDots += this->getKGray(pixel.red, 8);
                    }

                    if (i - 1 >= 0) {
                        pixel.load(this->buffer[(i - 1) * this->width + j]);
                        kSumDots += this->getKGray(pixel.red, 8);
                    }

                    if (i - 1 >= 0 && j + 1 < this->width) {
                        pixel.load(this->buffer[(i - 1) * this->width + j + 1]);
                        kSumDots += this->getKGray(pixel.red, 8);
                    }

                    if (j - 1 >= 0) {
                        pixel.load(this->buffer[i * this->width + j - 1]);
                        kSumDots += this->getKGray(pixel.red, 8);
                    }

                    if (j + 1 < this->width) {
                        pixel.load(this->buffer[i * this->width + j + 1]);
                        kSumDots += this->getKGray(pixel.red, 8);
                    }

                    if (i + 1 < this->height && j - 1 >= 0) {
                        pixel.load(this->buffer[(i + 1) * this->width + j - 1]);
                        kSumDots += this->getKGray(pixel.red, 8);
                    }

                    if (i + 1 < this->height) {
                        pixel.load(this->buffer[(i + 1) * this->width + j]);
                        kSumDots += this->getKGray(pixel.red, 8);
                    }

                    if (i + 1 < this->height && j + 1 < this->width) {
                        pixel.load(this->buffer[(i + 1) * this->width + j + 1]);
                        kSumDots += this->getKGray(pixel.red, 8);
                    }

                    if (kSumDots <= k) {
                        pixel.load((UCHAR) 0xFF);
                        this->preBitMap[i * this->width + j] = pixel.toInt32();
                    } else {
                        this->preBitMap[i * this->width + j] = curPixel.toInt32();
                    }
                }
            }

            memcpy(this->buffer, this->preBitMap, width * height * sizeof(int));
            free(this->preBitMap);
        }

        int getKGray(unsigned char color, int maxK) {
            // return maxK - ((color + 1) * maxK / 0x100);
            return (int) round(maxK * (1 - (float) (color + 1) / 0x100));
        }

        void analyseBitMap() {
            // do ...
        }
};
