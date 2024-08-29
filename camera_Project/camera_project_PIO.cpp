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

inline void cs_select()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_CS, 0); // Active low
  asm volatile("nop \n nop \n nop");
}

inline void cs_deselect()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_CS, 1);
  asm volatile("nop \n nop \n nop");
}

inline void DC_DATA()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_DC, 1); // data->high
  asm volatile("nop \n nop \n nop");
  sleep_us(100);
}

inline void DC_COMMAND()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_DC, 0); // command -> low
  asm volatile("nop \n nop \n nop");
  sleep_us(100);
}

inline void spi_transfer_byte(uint8_t byte)
{
  spi_write_blocking(SPI, &byte, 1);
}

inline void spi_transfer_word(uint16_t word)
{
  uint8_t tmp = word >> 8;
  uint8_t tmp2 = word & 0xff;
  spi_write_blocking(SPI, &tmp, 1);
  spi_write_blocking(SPI, &tmp2, 1);
  // spi_write16_blocking(m_spi, &word, 1);
}

void sendcommand(uint8_t IR)
{
  cs_select();
  DC_COMMAND();
  spi_transfer_byte(IR);
  cs_deselect();
}
int main()
{
    
    debug=false;

    const int w = QVGA_width;
    const int h = QVGA_height;
    int buf_size=w * h * 2;
    uint8_t buf[buf_size]={0,};
    PIO pio = pio0; // 사용할 PIO 모듈 선택
    uint32_t sm = 0; // 사용할 상태 머신 선택
    TFT_LCD tft;
    ov7670 cam(RAW_DATA);

    stdio_init_all();
    setup_pio(pio, sm, 1.0f , w, h);

    tft.init(SPI,SPI_baud);
    tft.color_screen(Green);
    tft.setAddrWindow(0,0,w,h);
    //tft.setBGR();//qvag
    tft.setRGB();//qqvga

    cam.ov7670_init(I2C,I2C_baud);
    cam.setResolution(resolution::QVGA);
    //cam.setImageType(IMG_Type::YUV);
    cam.setImageType(IMG_Type::RGB565);
    uint8_t *buf_p=buf;
    uint32_t t_width=2*w;
    while(1)
    {
        // //cam.getRawData<w, h>(buf);   
        // while (!(gpio_get(VS)));//wait for 1
        // while ((gpio_get(VS)));//wait for 0
        // while (!(gpio_get(VS)));//wait for 1
        // while ((gpio_get(VS)));//wait for 0

        // for(int i = 0; i < h; i++)
        // {
        //     // wait until horizontal data start
        //     while (!(gpio_get(HS)));//wait for 1
        //     for(int j = 0; j < t_width; j++)
        //     {
        //         while((gpio_get(PLK)));
        //         *buf_p++ = uint8_t(gpio_get_all() & 0xff);
        //         while(!(gpio_get(PLK)));
        //     }
        //     while ((gpio_get(HS)));
        // }   

        
        
        //setAddrWindow(0, 0, width, height);
        sendcommand(HX8357_RAMWR);
        cs_select();
        DC_DATA();
      
        for (int j = 0; j <buf_size; j++)
        {
            if (pio_sm_is_rx_fifo_empty(pio, sm)) {
                while(!pio_sm_is_rx_fifo_empty(pio, sm));
            }

            spi_transfer_byte(pio_sm_get(pio, sm));
            
        }
        cs_deselect();


        //tft.imshow(buf,w,h);
    
        //print_data(buf_size,buf);
    }
    
    return 0;
}



