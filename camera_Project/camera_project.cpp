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
#define baud 9600



int main()
{
    stdio_init_all();
    reset_IRQ_init(default_reset_Pin);
    debug=false;
    
    blink(10);
    ov7670 cam(QCIF_width,QCIF_height,default_Itype);
    cam.ov7670_init(I2C,baud);
    //cam.setImageType(IMG_Type::RGB565);
    
    Mat img = cam.getFrame();
    int num = img.data.size();
    printf("num=%d\n",num);

    for (int i=0; i<num; i++)
    {
        //printf("%x \t%x \t%x \t%x \n",img.data[i],img.data[i+1],img.data[i+2],img.data[i+3]);
        printf("%04x ",img.data[i]);
        if(i%150==0) printf("\n");
    }
    blink(10);
    printf("\nfinished");
    
    return 0;
}



