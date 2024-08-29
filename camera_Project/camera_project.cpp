// #include "C:/Users/spc/Documents/GitHub/raspberry-pi-pico-FPGA/libs/ImageProcessing.h"
// #include "C:/Users/spc/Documents/GitHub/raspberry-pi-pico-FPGA/libs/myfunc.h"
// #include "C:/Users/spc/Documents/GitHub/raspberry-pi-pico-FPGA/libs/ov7670.h"
#include "ImageProcessing.h"
#include "myfunc.h"
#include "ov7670.h"
#include <stdio.h>
#include "TFT.h"
#include "pico/stdlib.h" 
#include "pico/stdio_usb.h"
#include "hardware/spi.h"
#include "PIO_setting.h"
#include "read_ov7670.pio.h"

#define I2C i2c0
#define SPI spi0
#define I2C_baud 200*KHz
#define SPI_baud 60*MHz


int main()
{
    
    debug=false;
    const int w = QQVGA_width;
    const int h = QQVGA_height;
    int buf_size=w * h * 2;
    uint8_t buf[buf_size]={0,};
    TFT_LCD tft;
    ov7670 cam(RAW_DATA);

    stdio_init_all();

    tft.init(SPI,SPI_baud);
    tft.color_screen(Green);
    tft.setAddrWindow(0,0,w,h);
    //tft.setBGR();//qvag
    tft.setRGB();//qqvga

    cam.ov7670_init(I2C,I2C_baud);
    cam.setResolution(resolution::QQVGA);
    //cam.setImageType(IMG_Type::YUV);
    cam.setImageType(IMG_Type::RGB565);
    uint8_t *buf_p=buf;
    uint32_t t_width=2*w;
    while(1)
    {
        cam.getRawData<w, h>(buf); 
        tft.imshow(buf,w,h);
        //print_data(buf_size,buf);
    }
    
    return 0;
}



