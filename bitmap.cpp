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

            // first 4 bytes - size (width x height)
            int* size = (int*) malloc(sizeof(int));
            fread(size, sizeof(int), 1, file);
            this->width = *size & 0xFFFF;
            this->height = (*size >> 16) & 0xFFFF;
            free(size);

            size_t result;
            // stream size for read by parts
            int sizeStream = 100;
            int offset = 0;
            UCHAR byte = 0;

            if (width > 0 && height > 0) {
                this->bitMap = (int*) malloc(this->width * this->height * sizeof(int));
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

                free(stream);
            } while(result >= sizeStream);

            fclose(file);
        }

        int* getBitMap() {
            return this->bitMap;
        }

    private:
        int* bitMap;
};
