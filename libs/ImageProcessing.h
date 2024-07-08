#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include "pico/stdlib.h"
#include "myfunc.h"

enum IMG_Type
{
    RGB565=0,
    RGB888=1,
    GRAY=2
};

class Mat{
public:
    int16_t cols;
    int16_t rows;
    Array<uint16_t> data;
    Array<uint8_t> gray;
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