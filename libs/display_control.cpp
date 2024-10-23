#include "display_control.h"
#include "ImageProcessing.h"
#include "myfunc.h"
#include "ov7670.h"
#include "TFT.h"
#include "pico/stdlib.h" 
#include "pico/stdio_usb.h"
#include "hardware/spi.h"

//8bit-> I2C1, SPI->I2C0
#define I2C i2c1
#define SPI spi0
#define I2C_baud 200*KHz
#define SPI_baud 60*MHz

#define default_w QQVGA_width
#define default_h QQVGA_height

Display::Display()
{
    tft.init();
    tft.color_screen(Black);
    tft.setAddrWindow(0,0,default_w,default_h);
    tft.setRGB();//qqvga
    tft.setRotation(6);
    cam.ov7670_init(I2C,I2C_baud);
    cam.setResolution(resolution::QQVGA);
    cam.setImageType(IMG_Type::RGB565);
}
void Display::setRes_type(resolution res,IMG_Type type)
{
    if (res==resolution::QQVGA)
    {
        tft.setAddrWindow(0,0,QQVGA_width, QQVGA_height);
        tft.setRGB();
        cam.setResolution(QQVGA);
    }
    else if (res==resolution::QVGA)
    {
        tft.setAddrWindow(0,0,QVGA_width, QVGA_height);
        tft.setBGR();
        cam.setResolution(QVGA);
    }

    if(type==RGB565)
    {
        cam.setImageType(IMG_Type::RGB565);
    }
    else if(type==YUV)
    {
        cam.setImageType(IMG_Type::YUV);
    }
}