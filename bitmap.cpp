using namespace std;

class BitMap
{
    public:
        int width;
        int height;

        BitMap() {
            // do something...
        }

        BitMap(const char* fileName) {
            this->loadFile((char*) fileName);
        }

        BitMap(char* fileName) {
            this->loadFile(fileName);
        }

        void loadFile(char* fileName) {
            FILE* file = fopen(fileName, "rb");
            int* size = (int*) malloc(sizeof(int));
            fread(size, sizeof(int), 1, file);
            this->width = *size & 0xFFFF;
            this->height = (*size >> 16) & 0xFFFF;
            // vector<int*> fills;
            size_t result;
            int sizeStream = 100;
            int offset = 0;
            UCHAR byte = 0;

            if (width > 0 && height > 0) {
                cout << "Width x Height = " << this->width << " x " << this->height << endl;
                this->bitMap = (int*) malloc(this->width * this->height * sizeof(int));
            }

            int lastOffset = this->width * this->height;
            int pixel;

            do {
                UCHAR* stream = (UCHAR*) malloc(sizeStream * sizeof(UCHAR));
                memset(stream, 0, sizeStream * sizeof(UCHAR));

                result = fread(stream, sizeof(UCHAR), sizeStream, file);
                // int* interval = (int*) malloc(2 * sizeof(int));
                // result = fread(interval, sizeof(int), 2, file);
                // cout << "[" << fills.size() << "]: {" << interval[0] << " : " << interval[1] << "}" << endl;
                //cout << "Result: " << resuld << endl;

                if (result >= sizeStream) {
                    // fills.push_back(interval);
                    for (int i = 0; i < sizeStream; ++i) {
                        byte = stream[i];

                        for (int j = 0; j < 8; ++j) {
                            if ((byte & (0x01 << j)) == 0) {
                                pixel = 0xFFFFFF;
                            } else {
                                pixel = 0x000000;
                            }

                            if (lastOffset > offset + j) {
                                this->bitMap[offset + j] = pixel;
                            }
                        }

                        offset += 8;
                    }
                }
            } while(result >= sizeStream);

            

            /*int offset = 0;
            Pixel pixel;

            cout << fills.size() << " intervals" << endl;

            for (int i = 0; i < fills.size(); ++i) {
                int* curInterval = fills[i];

                // cout << "[" << i << "]: {" << curInterval[0] << " : " << curInterval[1] << "}" << endl;

                for (int j = offset; j < curInterval[0]; ++j) {
                    // pixel.load(0xFF);
                    this->bitMap[j] = 0xFFFFFF;
                }

                for (int j = 0; j < curInterval[1]; ++j) {
                    //pixel.load(0x00);
                    this->bitMap[curInterval[0] + j] = 0x000000;
                }

                offset = curInterval[0] + curInterval[1];
            }*/

            fclose(file);
        }

        int* getBitMap() {
            return this->bitMap;
        }

    private:
        int* bitMap;
};
