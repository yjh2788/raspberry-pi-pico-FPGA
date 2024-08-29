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
#include "pico/multicore.h"
#include "hardware/sync.h"
#include "pico/mutex.h"

// shared_memory와 ready 플래그를 atomic으로 설정


#define I2C i2c0
#define SPI spi0
#define I2C_baud 200*KHz
#define SPI_baud 60*MHz

constexpr int G_w = QVGA_width;
constexpr int G_h = QVGA_height;
constexpr int n = 262144;  // 2^18


volatile uint8_t shared_buffer[G_w * G_h * 2];
volatile bool data_ready = false;
mutex_t mutex;
TFT_LCD tft;
ov7670 cam(RAW_DATA);

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



// task for second core
void FrameSendToTFT() {
    while (true) {
        
        
        // read from shared memory
        // if (shared_memory.data_ready) {
        //     tft.imshow(shared_memory.image_data, G_w, G_h);
        //     shared_memory.data_ready = false;  // 데이터 처리 완료
        // }
        if (data_ready) {
            mutex_enter_blocking(&mutex);
            int total = G_w * 2 * G_h;
            sendcommand(HX8357_RAMWR);
            cs_select();
            DC_DATA();

            for (int j = 0; j < total; j++) {
                spi_transfer_byte(shared_buffer[j]);
            }
            cs_deselect();
            
            // 전송 완료 후 상태 플래그 리셋
            data_ready = false;
           
            mutex_exit(&mutex);
        }

       //sleep_ms(10);  
    }
}


int main()
{
    stdio_init_all();
    mutex_init(&mutex);
    multicore_launch_core1(FrameSendToTFT);
    debug=false;

    tft.init(SPI,SPI_baud);
    tft.color_screen(Green);
    
    cam.ov7670_init(I2C,I2C_baud);
    cam.setResolution(resolution::QVGA);
    //cam.setImageType(IMG_Type::YUV);
    cam.setImageType(IMG_Type::RGB565);
    
    int buf_size=G_w * G_h * 2;
    int t_width=G_w * 2;
    //Array<uint8_t> buf(buf_size, 0);
    uint8_t buf[buf_size]={0,};

    tft.setAddrWindow(0,0,G_w,G_h);
    tft.setBGR();//qvag
    //tft.setRGB();//qqvga
    
    uint8_t data=0;
    while(1)
    {
        while (!(gpio_get(VS)));//wait for 1
        while ((gpio_get(VS)));//wait for 0
        while (!(gpio_get(VS)));//wait for 1
        while ((gpio_get(VS)));//wait for 0

        for(int i = 0; i < G_h; i++)
        {
            // wait until horizontal data start
            while (!(gpio_get(HS)));//wait for 1
            for(int j = 0; j < t_width; j++)
            {
                while((gpio_get(PLK)));
                mutex_enter_blocking(&mutex);
                //while(!multicore_fifo_wready());
                shared_buffer[i * t_width + j] = uint8_t(gpio_get_all() & 0xff);
                //ready = true;
                mutex_exit(&mutex);
                while(!(gpio_get(PLK)));
            }
            while ((gpio_get(HS)));
        }
        mutex_enter_blocking(&mutex);
        data_ready = true;
        mutex_exit(&mutex);
        //print_data(buf_size,buf);
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



