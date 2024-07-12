
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

#define QVGA_width 320u
#define QVGA_height 240u
#define VGA_width 640u
#define VGA_height 480u
#define CIF_width 352u
#define CIF_height 288u
#define QCIF_width 176u
#define QCIF_height 144u

#define default_Itype IMG_Type::YUV
#define ov7670_I2C_ADDRESS 0x21
// The 7 bit SCCB/I2C address is 0x21, this translates to 0x42 for write address and 0x43 for read address.

enum resolution
{
    VGA = 0,  // 640 x 480 //RGB
    QVGA = 1, // 320 x 240 //RGB
    CIF = 2,  // 352 x 288  //YCbCr
    QCIF = 3  // 176 x 144 //YCbCr
};

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
    i2c_inst_t *m_i2c = nullptr;

    uint32_t pin_mask;
    uint32_t data_mask;
    uint8_t ov7670_address;
    uint8_t m_resolution=QVGA;
    uint8_t m_img_format=RGB565;
 
    ov7670();
    ov7670(uint width, uint height, IMG_Type Itype);
    void ov7670_pin_init();
    void ov7670_init(i2c_inst_t *i2c, uint32_t baudrate);
    Mat getFrame();
    Mat getFrame(uint16_t width, uint16_t height, IMG_Type Itype);
    uint8_t get_word_data();
    uint8_t sendCommand(const uint8_t* src);
    uint8_t sendCommand(const uint8_t reg, const uint8_t data);
    uint8_t readReg(const uint8_t reg);
    void setFormat(resolution res);
    void setImageType(IMG_Type Itype);
};


#endif