#ifndef MYFUNC_H
#define MYFUNC_H

#include "pico/binary_info.h"
#include "Array.h"

#define MASK(pin) 1ul << pin
#define default_reset_Pin 2

#define M_E        2.71828182845904523536   // e
#define M_LOG2E    1.44269504088896340736   // log2(e)
#define M_LOG10E   0.434294481903251827651  // log10(e)
#define M_LN2      0.693147180559945309417  // ln(2)
#define M_LN10     2.30258509299404568402   // ln(10)
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi
#define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#define M_SQRT2    1.41421356237309504880   // sqrt(2)
#define M_SQRT1_2  0.707106781186547524401

#define MHz 1000000
#define KHz 1000

typedef uint8_t	        U08;			/* data type definition */
typedef int8_t	        S08;
typedef uint16_t    	U16;
typedef int16_t 	    S16;
typedef uint32_t    	U32;
typedef int32_t 	    S32;

class Rect{
private:
   

public:
    Rect();
    Rect(int x, int y, int width, int hegiht);
    ~Rect();
    int x;
    int y;
    int width;
    int height;
};



extern bool debug;
float measure_duty_cycle(uint gpio);
int32_t pwm_set_freq_duty(uint32_t slice_num, uint32_t chan, uint32_t freq, int duty_cycle);
void blink(int num);
void debug_blink(int num);
void print_data(size_t size, uint8_t*buf);
void reset_callback(uint gpio, uint32_t events);
void reset_IRQ_init(uint pin=default_reset_Pin);
void debug_print(bool debug, const char *fmt, ...);
void debug_print(const char *fmt, ...);
void pinMode(int, int);
void debug_error_led();
bool wait_for_posedge(uint input_pin);
bool wait_for_negedge(uint input_pin);
#endif