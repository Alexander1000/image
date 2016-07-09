#define UCHAR unsigned char

using namespace std;

class BitMapCost
{
    public:
        int width;
        int height;

        BitMapCost() {
            // do something...
        }

        BitMapCost(const char* fileName) {
            this->loadFile((char*) fileName);
        }

        BitMapCost(char* fileName) {
            this->loadFile(fileName);
            this->prepareBuildCost(40);
            this->buildCostMap();
        }

        void loadFile(char* fileName) {
            FILE* file = fopen(fileName, "rb");

            // first 4 bytes - size (width x height)
            int* size = (int*) malloc(sizeof(int));
            fread(size, sizeof(int), 1, file);
            this->width = *size & 0xFFFF;
            this->height = (*size >> 16) & 0xFFFF;
            free(size);

            this->blackPixels = 0;

            size_t result;
            // stream size for read by parts
            int sizeStream = 100;
            int offset = 0;
            UCHAR byte = 0;

            if (width > 0 && height > 0) {
                cout << "Width x Height = " << this->width << " x " << this->height << endl;
                this->bitMap = (UCHAR*) malloc(this->width * this->height * sizeof(UCHAR));
                this->costMap = (float*) malloc(this->width * this->height * sizeof(float));
            } else {
                // invalid size
                return;
            }

            // maximal offset (width x height)
            int lastOffset = this->width * this->height;
            // code color (black 0x000000 or white 0xffffff)
            int pixel;

            do {
                // init memory for stream of bits
                UCHAR* stream = (UCHAR*) malloc(sizeStream * sizeof(UCHAR));
                memset(stream, 0, sizeStream * sizeof(UCHAR));

                result = fread(stream, sizeof(UCHAR), sizeStream, file);

                if (result >= sizeStream) {
                    for (int i = 0; i < sizeStream; ++i) {
                        byte = stream[i];

                        for (int j = 0; j < 8; ++j) {
                            if ((byte & (0x01 << j)) == 0) {
                                pixel = 1;
                                ++this->blackPixels;
                            } else {
                                pixel = 0;
                            }

                            if (lastOffset > offset + j) {
                                this->bitMap[offset + j] = pixel;
                            }
                        }

                        offset += 8;
                    }
                }

                free(stream);
            } while(result >= sizeStream);

            fclose(file);
        }

        UCHAR* getBitMap() {
            return this->bitMap;
        }

        void prepareBuildCost(int widthGradient) {
            this->widthGradient = widthGradient;

            for (int k = 0; k < this->widthGradient; ++k) {
                for (int i = 0; i < this->height; ++i) {
                    for (int j = 0; j < this->width; ++j) {
                        if (this->bitMap[i * this->width + j] != k + 1) {
                            continue;
                        }

                        if (this->isNull(i - 1, j - 1)) {
                            this->bitMap[(i - 1) * this->width + (j - 1)] = k + 2;
                        }

                        if (this->isNull(i - 1, j)) {
                            this->bitMap[(i - 1) * this->width + j] = k + 2;
                        }

                        if (this->isNull(i - 1, j + 1)) {
                            this->bitMap[(i - 1) * this->width + j + 1] = k + 2;
                        }

                        if (this->isNull(i, j - 1)) {
                            this->bitMap[i * this->width + j - 1] = k + 2;
                        }

                        if (this->isNull(i, j + 1)) {
                            this->bitMap[i * this->width + j + 1] = k + 2;
                        }

                        if (this->isNull(i + 1, j - 1)) {
                            this->bitMap[(i + 1) * this->width + j - 1] = k + 2;
                        } 

                        if (this->isNull(i + 1, j)) {
                            this->bitMap[(i + 1) * this->width + j] = k + 2;
                        }

                        if (this->isNull(i + 1, j + 1)) {
                            this->bitMap[(i + 1) * this->width + j + 1] = k + 2;
                        }
                    }
                }
            }
        }

        void buildCostMap() {
            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    this->costMap[i * this->width + j] = this->calcKoefficient(this->bitMap[i * this->width + j]);
                }
            }
        }

        void save(const char* fileName) {
            this->save((char*) fileName);
        }

        void save(char* fileName) {
            FILE* file = fopen(fileName, "wb");

            UCHAR* header = (UCHAR*) malloc(8 * sizeof(UCHAR));
            
            header[0] = (UCHAR) (this->width & 0xFF);
            header[1] = (UCHAR) ((this->width >> 8) & 0xFF);

            header[2] = (UCHAR) (this->height & 0xFF);
            header[3] = (UCHAR) ((this->height >> 8) & 0xFF);

            header[4] = (UCHAR) (this->blackPixels & 0xFF);
            header[5] = (UCHAR) ((this->blackPixels >> 8) & 0xFF);
            header[6] = (UCHAR) ((this->blackPixels >> 16) & 0xFF);
            header[7] = (UCHAR) ((this->blackPixels >> 24) & 0xFF);

            fwrite(header, 8, sizeof(UCHAR), file);

            int offset = 0;
            int step = 1024;
            int writeBlock = 0;
            int size = this->width * this->height;
            
            while (offset < size) {
                writeBlock = size - offset > step ? step : size - offset;
                fwrite(&this->costMap[offset], writeBlock, sizeof(float), file);
                offset += writeBlock;
            }

            fclose(file);
        }

    private:
        UCHAR* bitMap;
        float* costMap;
        int widthGradient;
        int blackPixels;

        float calcKoefficient(UCHAR index) {
            if (index == 0) {
                return 0.0f;
            }

            float diffE = exp(1) - 1.0;
            float indexK = (float) (this->widthGradient - index) / this->widthGradient;
            return log(1.0 + diffE * indexK);
        }

        bool isNull(int i, int j) {
            return i >= 0 && j >= 0 && i < this->height && j < this->width && this->bitMap[i * this->width + j] == 0;
        }
};
