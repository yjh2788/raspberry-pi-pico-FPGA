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
#define I2C i2c0
#define SPI spi0
#define baud 9600

int main()
{
    stdio_init_all();
    //reset_IRQ_init(default_reset_Pin);
    debug=false;
    TFT_LCD tft;
    tft.init(SPI,60*MHz);
    tft.color_screen(Green);
    blink(3);
    ov7670 cam(RAW_DATA);
    cam.ov7670_init(I2C,baud);
    cam.setResolution(resolution::QVGA);
    //cam.setImageType(IMG_Type::YUV);
    cam.setImageType(IMG_Type::RGB565);
    int w = QVGA_width;
    int h = QVGA_height;
    int buf_size=w * h * 2;
    //Array<uint8_t> buf(buf_size, 0);
    uint8_t buf[buf_size]={0,};
    tft.setAddrWindow(0,0,w,h);
    while(1)
    {
        if (cam.m_resolution == QQVGA) {    
            cam.getRawData<QQVGA_width, QQVGA_height>(buf);
        }else if (cam.m_resolution == QVGA) {
            cam.getRawData<QVGA_width, QVGA_height>(buf);
        } else if (cam.m_resolution == VGA) {
            cam.getRawData<VGA_width, VGA_height>(buf);
        } else if (cam.m_resolution == CIF) {
            cam.getRawData<CIF_width, CIF_height>(buf);
        } else if (cam.m_resolution == QCIF) {
            cam.getRawData<QCIF_width, QCIF_height>(buf);
        }
        tft.imshow(buf,w,h);
    
        //print_data(buf_size,buf.getbuf());
    }

  

    
    //printf("finished\n");
    // for (int i=0; i<num; i++)
    // {
    //     //printf("%x \t%x \t%x \t%x \n",img.data[i],img.data[i+1], img.data[i+2],img.data[i+3]);
    //     printf("%02x ",img.data8[i]);
    //     if(i%150==0) printf("\n");
    // }
    //blink(10);
    //debug_print(1,"\nfinished");
    
    return 0;
}



