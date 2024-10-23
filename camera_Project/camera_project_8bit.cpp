// #include "C:/Users/spc/Documents/GitHub/raspberry-pi-pico-FPGA/libs/ImageProcessing.h"
// #include "C:/Users/spc/Documents/GitHub/raspberry-pi-pico-FPGA/libs/myfunc.h"
// #include "C:/Users/spc/Documents/GitHub/raspberry-pi-pico-FPGA/libs/ov7670.h"
#include <string>
#include <stdio.h>
#include "display_control.h"
#include "pico/types.h"

int main()
{
    debug=false;
    const int w = QVGA_width;
    const int h = QVGA_height;
    int buf_size=w * h * 2;
    uint8_t buf[buf_size]={0,};
    uint8_t *buf_p=buf;
    uint32_t t_width=2*w;

    stdio_init_all();
    bus_ctrl_pin_init();

    Display disp;
    disp.setRes_type(QVGA,RGB565);
    
    while(1)
    {   
        absolute_time_t start_time = get_absolute_time(); // tick start
        CAM2PICO();
        disp.cam.getRawData<w, h>(buf); 
        PICO2LCD();
        disp.tft.imshow(buf, 0, 80, w, h);
        absolute_time_t end_time = get_absolute_time(); // tick end

       //calc time (ms)
        int fps = 1000000/(to_us_since_boot(end_time) - to_us_since_boot(start_time));
        disp.tft.setRotation(1);
        disp.tft.string_size(10,5,White,Black,(char*)"FPS:",2,2);
        disp.tft.string_size(60,5,White,Black,(char*)std::to_string(fps).c_str(),2,2);
        disp.tft.setRotation(6);
        printf("FPS: %d",fps);
        // tft.imshow(buf, w, 80, w, h);
        // tft.imshow(buf, 2 * w, 80, w, h);

        // tft.imshow(buf, 0, 80+h, w, h);
        // tft.imshow(buf, w, 80+h, w, h);
        // tft.imshow(buf, 2 * w, 80+h, w, h);

       
        //print_data(buf_size,buf);
    }
    
    return 0;
}



