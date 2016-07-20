#define UCHAR unsigned char

using namespace std;

class BitMapCost
{
    public:
        int width;
        int height;

        int kWidth;
        int kHeight;

        BitMapCost() {
            // do something...
        }

        BitMapCost(const char* fileName) {
            this->loadBinFile((char*) fileName);
        }

        BitMapCost(char* fileName) {
            this->loadBinFile(fileName);
        }

        float calc() {
            // float result = 0.0f;
            float sum = 0.0f;
            // int blackPixels = 0;

            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    if (this->bitMap[i * this->width + j] == 0) {
                        continue;
                    }

                    //++blackPixels;
                    int ic = (int) ((float) i * this->kHeight / this->height);
                    int jc = (int) ((float) j * this->kWidth / this->width);

                    float cost = this->costMap[ic * this->kWidth + jc]; 
                    sum += cost;
                }
            }

            int kForm = this->kWidth * this->kHeight;
            int form = this->width * this->height;
            float rateForm, ratePixels;

            if (kForm > form) {
                rateForm = (float) form / kForm;
            } else {
                rateForm = (float) kForm / form;
            }

            if (this->kBlackPixels > this->blackPixels) {
                ratePixels = (float) this->blackPixels / this->kBlackPixels;
            } else {
                ratePixels = (float) this->kBlackPixels / this->blackPixels;
            }
            
            return (sum / this->blackPixels) * rateForm * ratePixels;
        }

        void buildCost() {
            this->costMap = (float*) malloc(this->width * this->height * sizeof(float));
            this->prepareBuildCost(10);
            this->buildCostMap();
        }

        void loadKFile(char* fileName) {
            FILE* file = fopen(fileName, "rb");

            // first 4 bytes - size (width x height); 4 byte blackPixels
            int* size = (int*) malloc(sizeof(int));
            fread(size, sizeof(int), 1, file);
            this->kWidth = *size & 0xFFFF;
            this->kHeight = (*size >> 16) & 0xFFFF;
            free(size);

            fread(&this->kBlackPixels, sizeof(int), 1, file);

            size_t result;
            // stream size for read by parts
            int sizeStream = 128;
            int offset = 0;
            // float byte;

            if (width > 0 && height > 0) {
                this->costMap = (float*) malloc(this->kWidth * this->kHeight * sizeof(float));
            } else {
                // invalid size
                return;
            }

            do {
                // init memory for stream of bits
                float* stream = (float*) malloc(sizeStream * sizeof(float));
                memset(stream, 0, sizeStream * sizeof(float));

                result = fread(stream, sizeof(float), sizeStream, file);

                if (result > 0) {
                    memcpy(this->costMap + offset, stream, sizeStream * sizeof(float));
                    offset += result;
                }

                free(stream);
            } while(result > 0);

            fclose(file);
        }

        void loadBinFile(char* fileName) {
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
            int sizeStream = 128;
            int offset = 0;
            UCHAR byte = 0;

            if (width > 0 && height > 0) {
                this->bitMap = (UCHAR*) malloc(this->width * this->height * sizeof(UCHAR));
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

                if (result > 0) {
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
            } while(result > 0);

            fclose(file);
        }

        UCHAR* getBitMap() {
            return this->bitMap;
        }

        void prepareBuildCost(int widthGradient) {
            int minGradient = 0;
            this->heightGradient = (int) ceil((float) this->height / widthGradient);
            this->widthGradient = (int) ceil((float) this->width / widthGradient);

            if (this->heightGradient < this->widthGradient) {
                minGradient = this->heightGradient;
            } else {
                minGradient = this->widthGradient;
            }

            for (int k = 0; k < minGradient; ++k) {
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
            // прогреваем
            this->calcKoefficient(1);

            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    this->costMap[i * this->width + j] = this->calcKoefficient(this->bitMap[i * this->width + j]);
                }
            }

            /*
            for (int i = 0; i < this->widthGradient; ++i) {
                float cost = this->calcKoefficient(i);

                cout << "Cost[" << i << "] = " << cost << endl;
            }*/
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
        int heightGradient;
        int blackPixels;
        int kBlackPixels;
        float maxK;

        float calcKoefficient(UCHAR index) {
            if (index == 0) {
                return 0.0f;
            }

            int minGradient = 0;

            if (this->heightGradient < this->widthGradient) {
                minGradient = this->heightGradient;
            } else {
                minGradient = this->widthGradient;
            }

            float diffE = exp(1) - 1.0;
            float indexK = (float) (minGradient - index + 1) / minGradient;
            float k = exp(diffE * indexK) * sin(indexK * diffE);

            if (index == 1) {
                this->maxK = k;
            }

            return k / this->maxK;
        }

        bool isNull(int i, int j) {
            return i >= 0 && j >= 0 && i < this->height && j < this->width && this->bitMap[i * this->width + j] == 0;
        }
};
