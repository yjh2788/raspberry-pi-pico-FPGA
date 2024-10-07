
#include "BUS_control.h"
#include "pico/stdlib.h"

void bus_ctrl_pin_init()
{
    gpio_init(sel_pico_cam_pin);
    gpio_set_dir(sel_pico_cam_pin, GPIO_OUT);
    gpio_put(sel_pico_cam_pin, 0);
    
    gpio_init(sel_SIMD_data_pin);
    gpio_set_dir(sel_SIMD_data_pin, GPIO_OUT);
    gpio_put(sel_SIMD_data_pin, 0);

    gpio_init(BUS_DIR_pin);
    gpio_set_dir(BUS_DIR_pin, GPIO_OUT);
    gpio_put(BUS_DIR_pin, 0);

}
void PICO2LCD()
{
    gpio_set_dir_out_masked(0xff);
    gpio_pull_down(D0);
    gpio_pull_down(D1);
    gpio_pull_down(D2);
    gpio_pull_down(D3);
    gpio_pull_down(D4);
    gpio_pull_down(D5);
    gpio_pull_down(D6);
    gpio_pull_down(D7);
    gpio_put(BUS_DIR_pin, PICO_WR);
    gpio_put(sel_pico_cam_pin, sel_PICO);
    //sleep_us(50);
}
void CAM2PICO()
{
    gpio_set_dir_in_masked(0xff);
    gpio_put(BUS_DIR_pin, PICO_RD);
    gpio_put(sel_pico_cam_pin, sel_cam);
    gpio_put(sel_SIMD_data_pin,sel_p_c);
    gpio_pull_up(D0);
    gpio_pull_up(D1);
    gpio_pull_up(D2);
    gpio_pull_up(D3);
    gpio_pull_up(D4);
    gpio_pull_up(D5);
    gpio_pull_up(D6);
    gpio_pull_up(D7);

    //sleep_us(50);
}