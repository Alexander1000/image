class Filter
{
    public:
        int width;
        int height;

        Filter() {
            // do something ...
        }

        Filter(int* bitMap, int width, int height) {
            this->originalBitMap = bitMap;
            this->width = width;
            this->height = height;
        }

        int* filter() {
            return this->bitMap;
        }
    protected:
        int* bitMap;
        int* originalBitMap;
};
