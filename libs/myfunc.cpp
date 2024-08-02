#include "pico/stdlib.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "myfunc.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/watchdog.h"
#include "pico/divider.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// #include "hardware_watchdog/include/hardware/watchdog.h"
// #include "hardware_gpio/include/hardware/gpio.h"

bool debug=true;



int32_t pwm_set_freq_duty(uint32_t slice_num, uint32_t chan, uint32_t freq, 
                          int duty_cycle)
{

    uint8_t clk_divider = 0;
    uint32_t wrap = 0;
    uint32_t clock_div = 0;
    uint32_t clock = clock_get_hz(clk_sys);

    if(freq < 8 && 125000000> clock)
       /* This is the frequency range of generating a PWM 
       in RP2040 at 125MHz */
        return -1;

    for(clk_divider = 1; clk_divider < UINT8_MAX; clk_divider++)
    {
        /* Find clock_division to fit current frequency */
        clock_div = div_u32u32( clock, clk_divider );
        wrap = div_u32u32(clock_div, freq);
        if (div_u32u32 (clock_div, UINT16_MAX) <= freq && wrap <= UINT16_MAX)
        {
            break;
        }
    }
    if(clk_divider < UINT8_MAX)
    {
        /* Only considering whole number division */
        pwm_set_clkdiv_int_frac(slice_num, clk_divider, 0);
        pwm_set_enabled(slice_num, true);
        pwm_set_wrap(slice_num, (uint16_t) wrap);
        pwm_set_chan_level(slice_num, chan, 
                          (uint16_t) div_u32u32((((uint16_t)(duty_cycle == 100? 
                          (wrap + 1) : wrap)) * duty_cycle), 100));
    }
    else
        return -2;

    return 1;
}

float measure_duty_cycle(uint gpio) {
    // Only the PWM B pins can be used as inputs.
    assert(pwm_gpio_to_channel(gpio) == PWM_CHAN_B);
    uint slice_num = pwm_gpio_to_slice_num(gpio);

    // Count once for every 100 cycles the PWM B input is high
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_B_HIGH);
    pwm_config_set_clkdiv(&cfg, 100);
    pwm_init(slice_num, &cfg, false);
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    pwm_set_enabled(slice_num, true);
    sleep_ms(10);
    pwm_set_enabled(slice_num, false);
    float counting_rate = clock_get_hz(clk_sys) / 100;
    float max_possible_count = counting_rate * 0.01;
    return pwm_get_counter(slice_num) / max_possible_count;
}

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
        if((past != 0) && (now != 1))
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
        if((past != 1) && (now != 0))
            continue;
        else
            return 1;

        past=now;
        debug_print(debug,"waiting\n");

    }
}


void blink(int num)
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
void debug_blink(int num)
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
void print_data(size_t size, uint8_t*buf)
{
    printf("start\n");
    for(int i = 0;i< size; i++)
    {
        printf("%x ",buf[i]);
    }
    printf("\nfinish");
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

