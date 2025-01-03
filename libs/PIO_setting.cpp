#include "hardware/pio.h"
#include "hardware/dma.h"
#include "myfunc.h"
#include "read_ov7670.pio.h"
#include "PIO_setting.h"

// PIO program binary
extern const uint16_t read_ov7670_data_program_instructions[];


void setup_pio_and_dma(PIO pio, uint32_t sm, float clkdiv, uint32_t width, uint32_t height, uint32_t dma_channel,uint8_t *buf, uint32_t buf_size) {
    setup_pio(pio,  sm,  clkdiv,  width,  height);
    // DMA 설정
    dma_channel_config dma_cfg = dma_channel_get_default_config(dma_channel);
    channel_config_set_read_increment(&dma_cfg, true);
    channel_config_set_write_increment(&dma_cfg, true);
    channel_config_set_dreq(&dma_cfg, pio_get_dreq(pio, sm, true));
    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_8); // 8-bit transmition

    dma_channel_configure(
        dma_channel,    // DMA channel number
        &dma_cfg,       // DMA channel config
        buf,            // memory buffer (dst)
        &pio->rxf[sm],  // PIO FIFO (src)
        buf_size,       // transmition size
        false           // auto start flag
    );
    
}

bool dma_done(uint32_t dma_channel) {
    bool done = dma_hw->ints0 & (1u << dma_channel);
    dma_hw->ints0 = (1u << dma_channel); //request to clear done bit
    return done;
}

// PIO setting
void setup_pio(PIO pio, uint32_t sm, float clkdiv, uint32_t width, uint32_t height) {
    gpio_set_function(0, GPIO_FUNC_PIO0); 
    gpio_set_function(1, GPIO_FUNC_PIO0); 
    gpio_set_function(2, GPIO_FUNC_PIO0); 
    gpio_set_function(3, GPIO_FUNC_PIO0);
    gpio_set_function(4, GPIO_FUNC_PIO0); 
    gpio_set_function(5, GPIO_FUNC_PIO0); 
    gpio_set_function(6, GPIO_FUNC_PIO0); 
    gpio_set_function(7, GPIO_FUNC_PIO0); 
    gpio_set_function(11, GPIO_FUNC_PIO0); 
    gpio_set_function(12, GPIO_FUNC_PIO0); 
    gpio_set_function(13, GPIO_FUNC_PIO0);

    // PIO state machine init
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_clkdiv(&c, clkdiv); // 
    sm_config_set_in_pins(&c, 13); //VS
    sm_config_set_in_pins(&c, 12);//HS
    sm_config_set_in_pins(&c, 11);//PLK
    sm_config_set_in_pins(&c, 0);   // D0 
    pio_sm_set_consecutive_pindirs(pio, sm, 0, 8, false); // D0~D7 input pin setting

    pio_sm_put_blocking(pio, sm, width*2);
    pio_sm_put_blocking(pio, sm, height);

    // PIO program load
    pio_add_program(pio, &read_ov7670_data_program);
    pio_sm_init(pio, sm, pio_add_program(pio, &read_ov7670_data_program), &c);


    pio_sm_set_enabled(pio, sm, true);
}

uint16_t read_data(PIO pio, uint32_t sm) {
    // read from PIO FIFO
    return pio_sm_get(pio, sm);
}


void reset_pio(PIO pio, uint32_t sm, uint32_t width, uint32_t height) {
    pio_sm_config c = pio_get_default_sm_config();
    pio_sm_set_enabled(pio, sm, false); // disable state machine

    // remove the program
    uint32_t offset = pio_add_program(pio, &read_ov7670_data_program);
    pio_remove_program(pio, &read_ov7670_data_program, offset);

    // resolution setting
    pio_sm_put_blocking(pio, sm, width*2);
    pio_sm_put_blocking(pio, sm, height);

    // program load
    pio_add_program(pio, &read_ov7670_data_program);
    pio_sm_init(pio, sm, pio_add_program(pio, &read_ov7670_data_program), &c);

    pio_sm_set_enabled(pio, sm, true); // enable
}
