#include "ImageProcessing.h"

Mat::Mat()
{
    this->cols = 0;
    this->rows = 0;
    this->img_type = NULL;
}

Mat::Mat(uint16_t width, uint16_t height, IMG_Type Itype)
{
    this->cols = width;
    this->rows = height;
    this->img_type = Itype;
    if (Itype == IMG_Type::RGB565)
    {
        this->data = Array<uint16_t>::createArray(width * height);
    }
    else if ((Itype == IMG_Type::RGB888))
    {
        this->blue = Array<uint8_t>::createArray(width * height);
        this->red = Array<uint8_t>::createArray(width * height);
        this->green = Array<uint8_t>::createArray(width * height);
    }

    else if ((Itype == IMG_Type::GRAY))
    {
        this->gray = Array<uint8_t>::createArray(width * height);
    }
    else if (Itype == IMG_Type::YUV)
    {
        this->data = Array<uint16_t>::createArray(width * height);
    }
    else if (Itype == IMG_Type::RAW_DATA)
    {
        this->data8 = Array<uint8_t>::createArray(width * height*2);
    }
}

Mat::~Mat()
{
    if (this->data.size() != 0)
        this->data.freeArray();
    if (this->red.size() != 0)
        this->red.freeArray();
    if (this->blue.size() != 0)
        this->blue.freeArray();
    if (this->green.size() != 0)
        this->green.freeArray();
    if (this->gray.size() != 0)
        this->gray.freeArray();
}

// RGB565 to RGB888 conversion function
void Mat::rgb565_to_rgb888(uint16_t rgb565_color, uint8_t &red, uint8_t &green, uint8_t &blue)
{
    // Red channel extention
    red = ((rgb565_color >> 11) & 0x1F) << 3;

    // Green channel extention
    green = ((rgb565_color >> 5) & 0x3F) << 2;

    // Blue channel extention
    blue = ((rgb565_color >> 0) & 0x1F) << 3;
}

void Mat::rgb565_to_rgb888()
{
    // Red channel extention
    for (int i = 0; i < this->cols * this->rows; i++)
    {
        this->red[i] = ((data[i] >> 11) & 0x1F) << 3;

        // Green channel extention
        this->green[i] = ((data[i] >> 5) & 0x3F) << 2;

        // Blue channel extention
        this->blue[i] = ((data[i] >> 0) & 0x1F) << 3;
    }
}