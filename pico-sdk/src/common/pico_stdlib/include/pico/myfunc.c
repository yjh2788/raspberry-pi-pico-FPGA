#include "pico/stdlib.h"
void pinMode(int led_pin,int mode )
{
    int led_pin;
    int mode;
    if (mode){gpio_set_dir(led_pin, GPIO_in;}
    else {gpio_set_dir(LED_PIN, GPIO_OUT);}
    
}