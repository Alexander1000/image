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

            cout << "Width x Height = " << this->width * this->height << endl;
            cout << "Size of bin file: " << this->size << endl;

            this->bitMap[0] = (UCHAR) (width & 0xFF);
            this->bitMap[1] = (UCHAR) ((width >> 8) & 0xFF);

            this->bitMap[2] = (UCHAR) (height & 0xFF);
            this->bitMap[3] = (UCHAR) ((height >> 8) & 0xFF);

            Pixel pixel;

            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    pixel.load(this->originalBitMap[i * this->width + j]);
                    this->setPixel(i, j, pixel.red > 0 ? 1 : 0);
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
            int step = 1000;
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

        void setPixel(int line, int col, UCHAR value) {
            int offsetPixel = (line * this->width + col);
            int offsetBytes = offsetPixel / 8 + 4;
            int modPixel = offsetPixel % 8;
            this->bitMap[offsetBytes] |= (value & 0x01) << modPixel;
        }

        int getKGray(UCHAR color, int maxK) {
            return (int) round(maxK * (1 - (float) (color + 1) / 0x100));
        }
};
