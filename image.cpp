// class for save & load image
class Image
{
    public:
        // ширина и высота
        int width;
        int height;

        // глубина цвета
        int depth;

        // битовая карта изображения
        int *bitMap;

        // original file
        char* fileName;

        Image() {
            // do something ...
        }

        Image(const char* fileName) {
            this->loadJpeg(fileName);
        }

        int loadJpeg(const char* fileName) {
            return this->loadJpeg((char*) fileName);
        }

        int loadJpeg(char* fileName) {
            this->fileName = fileName;

            unsigned char a, r, g, b;
            // int width, height;
            struct jpeg_decompress_struct cinfo;
            struct jpeg_error_mgr jerr;

            FILE * infile;        /* source file */
            JSAMPARRAY pJpegBuffer;       /* Output row buffer */
            int row_stride;       /* physical row width in output buffer */

            if ((infile = fopen(this->fileName, "rb")) == NULL) {
                fprintf(stderr, "can't open %s\n", this->fileName);
                return -1;
            }

            cinfo.err = jpeg_std_error(&jerr);
            jpeg_create_decompress(&cinfo);
            jpeg_stdio_src(&cinfo, infile);
            (void) jpeg_read_header(&cinfo, TRUE);
            (void) jpeg_start_decompress(&cinfo);

            // получаем длину и ширино изображения
            this->width = cinfo.output_width;
            this->height = cinfo.output_height;

            unsigned char * pDummy = new unsigned char [this->width * this->height * 4];
            unsigned char * pTest = pDummy;

            if (!pDummy) {
                printf("NO MEM FOR JPEG CONVERT!\n");
                return -1;
            }

            row_stride = this->width * cinfo.output_components;
            pJpegBuffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

            while (cinfo.output_scanline < cinfo.output_height) {
                (void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
                for (int x = 0; x < width; x++) {
                    a = 0; // alpha value is not supported on jpg
                    r = pJpegBuffer[0][cinfo.output_components * x];

                    if (cinfo.output_components > 2) {
                        g = pJpegBuffer[0][cinfo.output_components * x + 1];
                        b = pJpegBuffer[0][cinfo.output_components * x + 2];
                    } else {
                        g = r;
                        b = r;
                    }

                    *(pDummy++) = r;
                    *(pDummy++) = g;
                    *(pDummy++) = b;
                    *(pDummy++) = a;
                }
            }

            fclose(infile);

            (void) jpeg_finish_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);

            this->bitMap = (int*) pTest; 
            this->depth = 32;

            return 0;
        }

        int saveJpeg(const char* fileName) {
            return this->saveJpeg((char*) fileName);
        }

        int saveJpeg(char* fileName) {
            return this->saveJpegImage(fileName, this->bitMap, this->width, this->height);
        }

        int saveJpegImage(char* fileName, int* bitMap, int width, int height) {
            struct jpeg_compress_struct cinfo;
            struct jpeg_error_mgr jerr;

            /* this is a pointer to one row of image data */
            JSAMPROW rowPointer[1];
            FILE* outFile = fopen(fileName, "wb");

            if (!outFile) {
                printf("Error opening output jpeg file %s\n!", fileName );
                return -1;
            }

            cinfo.err = jpeg_std_error(&jerr);
            jpeg_create_compress(&cinfo);
            jpeg_stdio_dest(&cinfo, outFile);

            /* Setting the parameters of the output file here */
            cinfo.image_width = width;  
            cinfo.image_height = height;
            cinfo.input_components = 3;
            cinfo.in_color_space = JCS_RGB;

            /* default compression parameters, we shouldn't be worried about these */
            jpeg_set_defaults(&cinfo);

            /* Now do the compression .. */
            jpeg_start_compress(&cinfo, TRUE);

            /* like reading a file, this time write one row at a time */
            while(cinfo.next_scanline < cinfo.image_height) {
                unsigned char* imageRow = (unsigned char*) malloc(cinfo.image_width * sizeof(char) * cinfo.input_components);

                for (int j = 0; j < cinfo.image_width; ++j) {
                    int pixel = bitMap[cinfo.next_scanline * cinfo.image_width + j];

                    imageRow[j * cinfo.input_components] = (unsigned char) (pixel & 0xFF);
                    imageRow[j * cinfo.input_components + 1] = (unsigned char) ((pixel >> 8) & 0xFF);
                    imageRow[j * cinfo.input_components + 2] = (unsigned char) ((pixel >> 16) & 0xFF);
                }

                rowPointer[0] = imageRow;
                jpeg_write_scanlines(&cinfo, rowPointer, 1);
            }
            /* similar to read file, clean up after we're done compressing */
            jpeg_finish_compress(&cinfo);
            jpeg_destroy_compress(&cinfo);
            fclose(outFile);

            return 0;
        }
};
