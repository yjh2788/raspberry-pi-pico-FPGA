
#ifndef OV7670_H
#define OV7670_H
#include "ov7670_reg.h"
#include "pico/stdlib.h"
#include "myfunc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "ImageProcessing.h"
#include "pico/binary_info/code.h"

// respberry pico gpio pin allocation
#define SCL 1
#define SDA 0
#define VS 26
#define HS 7
#define PLK 22
#define XLK 9
#define D0 13
#define D1 18
#define D2 12
#define D3 19
#define D4 11
#define D5 20
#define D6 10
#define D7 21

#define default_width 640
#define default_height 420
#define default_Itype IMG_Type::RGB565

class ov7670
{
public:
    uint m_SCL;
    uint m_SDA;
    uint m_VS;
    uint m_HS;
    uint m_PLK;
    uint m_XLK;
    uint m_D0;
    uint m_D1;
    uint m_D2;
    uint m_D3;
    uint m_D4;
    uint m_D5;
    uint m_D6;
    uint m_D7;
    Mat m_img;

    uint32_t pin_mask;
    uint32_t data_mask;

    ov7670();
    ov7670(uint width, uint height, IMG_Type Itype);
    void ov7670_pin_init();
    void ov7670_init(i2c_inst_t *i2c, uint32_t baudrate);
    Mat getFrame();
    Mat getFrame(uint16_t width, uint16_t height, IMG_Type Itype);
    uint8_t get_word_data();

};


#endif