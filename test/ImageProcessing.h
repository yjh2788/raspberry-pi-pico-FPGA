#include "pico/stdlib.h"
#include "myfunc.h"
#include <malloc.h>


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
    uint16_t* data=nullptr;
    uint8_t* gray=nullptr;
    uint8_t* red=nullptr;
    uint8_t* green=nullptr;
    uint8_t* blue=nullptr; 
    bool img_type;
    Mat(uint16_t width, uint16_t height, enum IMG_Type);
    Mat();
    ~Mat();

    void rgb565_to_rgb888(uint16_t rgb565_color, uint8_t &red, uint8_t &green, uint8_t &blue);
    void rgb565_to_rgb888();
};