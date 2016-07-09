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
            // this->size = 4;
            this->bitMap = (UCHAR*) malloc(this->size * sizeof(UCHAR));
            memset(this->bitMap, 0, this->size);

            // cout << "Width x Height = " << this->width * this->height << endl;
            // cout << "Size of bin file: " << this->size << endl;

            this->bitMap[0] = (UCHAR) (width & 0xFF);
            this->bitMap[1] = (UCHAR) ((width >> 8) & 0xFF);

            this->bitMap[2] = (UCHAR) (height & 0xFF);
            this->bitMap[3] = (UCHAR) ((height >> 8) & 0xFF);

            Pixel pixel;
            // int count = 0;
            // int blackOffset = 0;
            // vector<int*> fills;

            // bool isBlask = false;
            UCHAR byte = 0;
            int offset = 0;

            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    pixel.load(this->originalBitMap[i * this->width + j]);
                    offset = i * this->width + j;
                    // this->setPixel(i, j, pixel.red > 0 ? 1 : 0);
                    // test = onBlack ? pixel.red == 0 : pixel.red > 0;

                    if (pixel.red < 32) {
                        byte |= (0x01 << (offset % 8));
                    }

                    if ((offset + 1) % 8 == 0) {
                        this->bitMap[4 + ((offset + 1) / 8) - 1] = byte;
                        byte = 0;
                    }

                    /*if (pixel.red < 32 && !isBlask) {
                        isBlask = true;
                        count = 1;
                        blackOffset = i * this->width + j;
                        continue;
                    }

                    if (pixel.red < 32 && isBlask) {
                        ++count;
                        continue;
                    }

                    if (pixel.red >= 32 && isBlask) {
                        isBlask = false;
                        int* info = (int*) malloc(2 * sizeof(int));
                        info[0] = blackOffset;
                        info[1] = count;
                        fills.push_back(info);
                        count = 0;
                        continue;
                    }*/
                }
            }

            /*int curOffset = 4;

            for (int i = 0; i < fills.size(); ++i) {
                int* curInterval = fills[i];
                UCHAR* newBitMap = (UCHAR*) malloc((this->size + 8) * sizeof(UCHAR));
                memset(newBitMap, 0, this->size + 8);
                memcpy(newBitMap, this->bitMap, this->size);
                free(this->bitMap);
                this->bitMap = newBitMap;
                this->size += 8;

                cout << "Offset: " << curInterval[0] << "; Count: " << curInterval[1] << endl;

                for (int j = 0; j < 4; ++j) {
                    this->bitMap[curOffset + j] = ((curInterval[0] >> j * 8) & 0xFF);
                    this->bitMap[curOffset + 4 + j] = ((curInterval[1] >> j * 8) & 0xFF);
                }

                curOffset += 8;
                free(curInterval);
            }*/

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
