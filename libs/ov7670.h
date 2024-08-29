
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
#define SCL 9
#define SDA 8
#define VS 13
#define HS 12
#define PLK 11
#define XLK 10
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

#define QQVGA_width 160u
#define QQVGA_height 120u
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



class ov7670{
public:
    uint32_t m_SCL;
    uint32_t m_SDA;
    uint32_t m_VS;
    uint32_t m_HS;
    uint32_t m_PLK;
    uint32_t m_XLK;
    uint32_t m_D0;
    uint32_t m_D1;
    uint32_t m_D2;
    uint32_t m_D3;
    uint32_t m_D4;
    uint32_t m_D5;
    uint32_t m_D6;
    uint32_t m_D7;
    Mat m_img;
    i2c_inst_t *m_i2c = nullptr;
    uint8_t m_resolution=QVGA;
    uint8_t m_img_format=RGB565;
 
    ov7670();
    ov7670(IMG_Type Itype);
    ov7670(uint32_t width, uint32_t height, IMG_Type Itype);
    void ov7670_xclk_init(uint32_t freq,int duty);
    void ov7670_pin_init();
    void ov7670_PIO_pin_init();
    void ov7670_init(i2c_inst_t *i2c, uint32_t baudrate);
    void ov7670_init(i2c_inst_t *i2c, uint32_t baudrate, bool PIO_enable);
    template<int width,int height>
    void getRawData(uint8_t *buf);
    template<int width,int height>
    Mat getFrame();
    uint8_t sendCommand_regs(const uint8_t src[][2]);
    uint8_t sendCommand(const uint8_t* src);
    uint8_t sendCommand(const uint8_t reg, const uint8_t data);
    uint8_t readReg(const uint8_t reg);
    void setResolution(resolution res);
    void setImageType(IMG_Type Itype);

private:
    uint32_t pin_mask;
    uint32_t data_mask;
    uint8_t get_word_data();
    uint8_t get_word_data2();
    uint8_t get_word_data3();
};

/// @brief width, height should be declared as a constant because of timing issue depends on compiler optimization
///        if widh, height is declared as variable, there might be timing issue when to read data from ov7670
/// @tparam width 
/// @tparam height 
/// @param buf 
template<int width,int height>
void ov7670::getRawData(uint8_t * buf)
{
    while (!(gpio_get(m_VS)));//wait for 1
    while ((gpio_get(m_VS)));//wait for 0
    while (!(gpio_get(m_VS)));//wait for 1
    while ((gpio_get(m_VS)));//wait for 0

    for(int i = 0; i < height; i++)
    {
        // wait until horizontal data start
        while (!(gpio_get(m_HS)));//wait for 1
        for(int j = 0; j < width * 2; j++)
        {
            while((gpio_get(m_PLK)));
            *buf++ = get_word_data(); // high
            while(!(gpio_get(m_PLK)));
        }
        while ((gpio_get(m_HS)));
    }
}

template<int width,int height>
Mat ov7670::getFrame()
{
    Mat img;
    uint8_t *buf=img.data.getbuf();
    while (!(gpio_get(m_VS)));//wait for 1
    while ((gpio_get(m_VS)));//wait for 0
    while (!(gpio_get(m_VS)));//wait for 1
    while ((gpio_get(m_VS)));//wait for 0

    for(int i = 0; i < height; i++)
    {
        // wait until horizontal data start
        while (!(gpio_get(m_HS)));//wait for 1
        for(int j = 0; j < width * 2; j++)
        {
            while((gpio_get(m_PLK)));
            *buf++ = get_word_data(); // high
            while(!(gpio_get(m_PLK)));
        }
        while ((gpio_get(m_HS)));
    }
    return img;
}

// //template function decared
// template void ov7670::print_RAW8bitdata<QVGA_height, QVGA_width>(uint8_t*);
// template void ov7670::print_RAW8bitdata<VGA_height, VGA_width>(uint8_t*);
// template void ov7670::print_RAW8bitdata<CIF_height, CIF_width>(uint8_t*);
// template void ov7670::print_RAW8bitdata<QCIF_height, QCIF_width>(uint8_t*);
#endif