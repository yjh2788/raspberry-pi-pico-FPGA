#include "pico/stdlib.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "myfunc.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/watchdog.h"

// #include "hardware_watchdog/include/hardware/watchdog.h"
// #include "hardware_gpio/include/hardware/gpio.h"

bool debug=true;

void pinMode(int led_pin, int mode)
{
    if (mode)
    {
        gpio_set_dir(led_pin, GPIO_IN);
    }
    else
    {
        gpio_set_dir(led_pin, GPIO_OUT);
    }
}

void debug_print(bool debug, const char *fmt, ...) {
    if (debug) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}
void debug_print(const char *fmt, ...) {
    if (debug) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}
void debug_error_led()
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    //gpio_set_dir(LED_PIN, GPIO_OUT);
    pinMode(LED_PIN,0);
    for(int i = 0; i < 5 ; i++)
    {
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);
     }
}


bool wait_for_posedge(uint input_pin)
{
    bool past=gpio_get(input_pin);
    
    while(1)
    {
        bool now=gpio_get(input_pin);
        if(past != 0 && now != 1)
            continue;
        else
            return 1;

        past=now;
        debug_print(debug,"waiting\n");

    }
}

bool wait_for_negedge(uint input_pin)
{
    bool past=gpio_get(input_pin);
    
    while(1)
    {
        bool now=gpio_get(input_pin);
        if(past != 1 && now != 0)
            continue;
        else
            return 1;

        past=now;
        debug_print(debug,"waiting\n");

    }
}


void blink(int num)
{
    if (debug)
    {
        const uint LED_PIN = PICO_DEFAULT_LED_PIN;
        gpio_init(LED_PIN);
        //gpio_set_dir(LED_PIN, GPIO_OUT);
        pinMode(LED_PIN,0);
        gpio_put(LED_PIN, 0);
        sleep_ms(1000);

        for (int i =0; i<num;i++)
        {
            gpio_put(LED_PIN, 1);
            sleep_ms(250);
            gpio_put(LED_PIN, 0);
            sleep_ms(250);
        }
    }
}

void reset_callback(uint gpio, uint32_t events)
{
    watchdog_reboot(0, 0, 0);
}
void reset_IRQ_init(uint pin)
{
    gpio_init(pin);
    gpio_set_dir(pin,GPIO_IN);
    gpio_pull_up(pin);
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL, true, &reset_callback);
}

