#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include "pico/stdlib.h"
#include "myfunc.h"

enum IMG_Type
{
    RGB565 = 0,
    RGB888 = 1,
    YUV = 2,
    RAW_RGB= 3,
    p_RAW_RGB = 4,
    GRAY = 5,
    RAW_DATA = 6

};

enum resolution
{
    VGA = 0,  // 640 x 480 //RGB
    QVGA = 1, // 320 x 240 //RGB
    QQVGA = 2, // 160 x 120 //RGB
    CIF = 3,  // 352 x 288  //YCbCr
    QCIF = 4  // 176 x 144 //YCbCr
};

class Mat
{
public:
    int16_t cols;
    int16_t rows;
    Array<uint8_t> gray;
    Array<uint8_t> data;
    Array<uint8_t> red;
    Array<uint8_t> green;
    Array<uint8_t> blue;
    bool img_type;
    Mat(uint16_t width, uint16_t height, IMG_Type);
    Mat();
    ~Mat();

    void rgb565_to_rgb888(uint16_t rgb565_color, uint8_t &red, uint8_t &green, uint8_t &blue);
    void rgb565_to_rgb888();
};

#endif