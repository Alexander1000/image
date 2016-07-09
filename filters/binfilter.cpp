class BinFilter : public Filter
{
    public:
        BinFilter() : Filter() {
            // do something ...
        }

        BinFilter(int* bitMap, int width, int height) : Filter(bitMap, width, height) {
            // do something ...
         }

        UCHAR* filter() {
            // 1 пиксель = 2 возможных состояний (1 бит)
            this->size = (ceil(width * height / 8) + 4);
            this->bitMap = (UCHAR*) malloc(this->size * sizeof(UCHAR));
            memset(this->bitMap, 0, this->size);

            // save image size
            this->bitMap[0] = (UCHAR) (width & 0xFF);
            this->bitMap[1] = (UCHAR) ((width >> 8) & 0xFF);

            this->bitMap[2] = (UCHAR) (height & 0xFF);
            this->bitMap[3] = (UCHAR) ((height >> 8) & 0xFF);

            Pixel pixel;
            UCHAR byte = 0;
            int offset = 0;

            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    pixel.load(this->originalBitMap[i * this->width + j]);
                    // offset pixel
                    offset = i * this->width + j;

                    if (pixel.red < 32) {
                        // if black pixel then set 1 bit
                        byte |= (0x01 << (offset % 8));
                    }

                    if ((offset + 1) % 8 == 0) {
                        // save every 8 pixel in bitmap
                        this->bitMap[4 + ((offset + 1) / 8) - 1] = byte;
                        byte = 0;
                    }
                }
            }

            return this->bitMap;
        }

        int save(const char* fileName) {
            return this->save((char*) fileName);
        }

        int save(char* fileName) {
            FILE* file = fopen(fileName, "wb");
            int offset = 0;
            int step = 1024;
            int writeBlock = 0;
            
            while (offset < this->size) {
                writeBlock = this->size - offset > step ? step : this->size - offset;
                fwrite(&this->bitMap[offset], writeBlock, sizeof(UCHAR), file);
                offset += writeBlock;
            }

            fclose(file);
            return 0;
        }

    private:
        int size;
        UCHAR* bitMap;
};
