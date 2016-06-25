#define UCHAR unsigned char

class Pixel
{
    public:
        UCHAR red;
        UCHAR green;
        UCHAR blue;
        UCHAR alpha;

        Pixel() {}

        Pixel(int color) {
            this->load(color);
        }

        Pixel(UCHAR greyColor) {
            this->load(greyColor);
        }

        Pixel(UCHAR red, UCHAR green, UCHAR blue) {
            this->load(red, green, blue);
        }

        Pixel(UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha) {
            this->load(red, green, blue, alpha);
        }

        void load(int color) {
            this->red = (UCHAR) (color & 0xFF);
            this->green = (UCHAR) ((color >> 8) & 0xFF);
            this->blue = (UCHAR) ((color >> 16) & 0xFF);
            this->alpha = (UCHAR) ((color >> 24) & 0xFF);
        }

        void load(UCHAR greyColor) {
            this->red = greyColor;
            this->green = greyColor;
            this->blue = greyColor;
            this->alpha = 0;
        }

        void load(UCHAR red, UCHAR green, UCHAR blue) {
            this->red = red;
            this->green = green;
            this->blue = blue;
        }

        void load(UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha) {
            this->red = red;
            this->green = green;
            this->blue = blue;
            this->alpha = alpha;
        }

        int toInt32() {
            return this->red + (this->green << 8) + (this->blue << 16) + (this->alpha << 24);
        }
};
