#ifndef MYFUNC_H
#define MYFUNC_H
#include "pico/binary_info.h"

#define MASK(pin) 1ul << pin

typedef signed char int8_t;
typedef unsigned char   uint8_t;
typedef short  int16_t;
typedef unsigned short  uint16_t;
typedef int  int32_t;
typedef unsigned   uint32_t;
typedef unsigned int uint;

bool debug=true;
void debug_print(bool debug, const char *fmt, ...);
void pinMode(int, int);
void debug_error_led();
bool wait_for_posedge(uint input_pin);
bool wait_for_negedge(uint input_pin);
#endif