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
#include "hardware/dma.h"
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
    uint8_t *buf_p=buf;
    uint32_t t_width=2*w;
    uint32_t dma_channel=dma_claim_unused_channel(true);
    
    uint32_t sm = 0; // state machine
    PIO pio = pio0; // PIO 
    TFT_LCD tft;
    ov7670 cam(RAW_DATA);

    stdio_init_all();
    setup_pio(pio, sm, 1.0f , w, h);
    //setup_pio_and_dma(pio, sm, 1.0f , w, h, dma_channel,buf,buf_size);
    
    tft.init(SPI,SPI_baud);
    tft.color_screen(Green);
    tft.setAddrWindow(0,0,w,h);
    //tft.setBGR();//qvag
    tft.setRGB();//qqvga

    cam.ov7670_init(I2C,I2C_baud, 1);
    cam.setResolution(resolution::QVGA);
    //cam.setImageType(IMG_Type::YUV);
    cam.setImageType(IMG_Type::RGB565);
    //dma_channel_start(dma_channel);
    while(1)
    {
      
        // DMA 전송 완료 대기
        //while (!dma_done(dma_channel));
        //dma_channel_wait_for_finish_blocking(dma_channel);

        // sendcommand(HX8357_RAMWR);
        // cs_select();
        // DC_DATA();
      
        for (int j = 0; j <buf_size; j+=4)
        {
            if (pio_sm_is_rx_fifo_empty(pio, sm)) {
                while(!pio_sm_is_rx_fifo_empty(pio, sm));
            }
            uint32_t data = pio_sm_get(pio,sm);
            printf("data=%x'n",data);
            buf_p[j]=data&0xff;
            buf_p[j+1]=(data>>8)&0xff;
            buf_p[j+2]=(data>>16)&0xff;
            buf_p[j+3]=(data>>24)&0xff;
        }
        //     spi_transfer_word((data>>16)&0xffff);
        //     spi_transfer_word(data&0xffff);
            
        // }
        // cs_deselect();
       // blink(1);
        tft.imshow(buf,w,h);
        //blink(1);
    
        //print_data(buf_size,buf);
    }
    
    return 0;
}



