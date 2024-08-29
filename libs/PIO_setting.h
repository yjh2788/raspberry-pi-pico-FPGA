#ifndef PIO_SETTING_H
#define PIO_SETTING_H
#include "hardware/pio.h"
#include "myfunc.h"


void setup_pio(PIO pio, uint32_t sm, float clkdiv, uint32_t width, uint32_t height) ;
uint16_t read_data(PIO pio, uint32_t sm) ;
void reset_pio(PIO pio, uint32_t sm, uint32_t width, uint32_t height);

#endif