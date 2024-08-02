#include <stdio.h>
#include "C:\pico\pico-sdk\src\rp2_common\rpico\include\rpico.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#define gpio_in 1
#define gpio_out 0
#define OUTPUT 0
#define INPUT 1

void pinMode(int pin,int mode )
{
    gpio_init(pin);
    if (mode){gpio_set_dir(pin, GPIO_IN);}
    else {gpio_set_dir(pin, GPIO_OUT);}
    
}


void digitalWrite(int pin, int bin)
{
	gpio_put(pin, bin);
}

int digitalRead(int pin)
{
    int value;
    gpio_set_input_enabled(pin,true);
    value = gpio_get(pin);
    return value;
}


void analogWrite(int pin,int value,int max)
{
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin); 
    pwm_set_wrap(slice_num, max);
    pwm_set_gpio_level(pin,value);
    pwm_set_enabled(slice_num, true);
}

uint16_t analogRead(int pin)
{
    int a_pin=pin-26;
    uint16_t output;
    adc_init();
    adc_gpio_init(pin);
    adc_select_input(a_pin);
    output = adc_read();  
    return output;
}



