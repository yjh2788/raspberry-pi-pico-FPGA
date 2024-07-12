#ifndef MYFUNC_H
#define MYFUNC_H

#include "pico/binary_info.h"
#include "Array.h"

#define MASK(pin) 1ul << pin
#define default_reset_Pin 2

// typedef signed char int8_t;
// typedef unsigned char   uint8_t;
// typedef short  int16_t;
// typedef unsigned short  uint16_t;
// typedef int  int32_t;
// typedef unsigned   uint32_t;
// typedef unsigned int uint;


extern bool debug;
void blink(int num);
void reset_callback(uint gpio, uint32_t events);
void reset_IRQ_init(uint pin=default_reset_Pin);
void debug_print(bool debug, const char *fmt, ...);
void debug_print(const char *fmt, ...);
void pinMode(int, int);
void debug_error_led();
bool wait_for_posedge(uint input_pin);
bool wait_for_negedge(uint input_pin);
#endif