// #include "C:/Users/spc/Documents/GitHub/raspberry-pi-pico-FPGA/libs/ImageProcessing.h"
// #include "C:/Users/spc/Documents/GitHub/raspberry-pi-pico-FPGA/libs/myfunc.h"
// #include "C:/Users/spc/Documents/GitHub/raspberry-pi-pico-FPGA/libs/ov7670.h"
#include "ImageProcessing.h"
#include "myfunc.h"
#include "ov7670.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio_usb.h"

#define I2C i2c0
#define baud 115200

int main()
{
    stdio_init_all();
    ov7670 cam(default_width,default_height,default_Itype);
    //Mat img;
    cam.ov7670_init(I2C,baud);
    Mat img=cam.getFrame();
    int num=img.data.size();

    for (int i=0; i<num; i++)
    {
        printf("%x",img.data[i]);
    }

    return 0;
}