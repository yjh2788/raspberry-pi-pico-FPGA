#ifndef BUS_CONTROL_H
#define BUS_CONTROL_H


#define sel_pico_cam_pin  9
#define sel_SIMD_data_pin 8
#define BUS_DIR_pin 20

#define PICO_WR 0
#define PICO_RD 1

#define sel_PICO 0
#define sel_cam 1

#define sel_p_c 0
#define sel_simd 1

#define D0  0
#define D1  1
#define D2  2
#define D3  3
#define D4  4
#define D5  5
#define D6  6
#define D7  7

void bus_ctrl_pin_init();
void PICO2LCD();
void CAM2PICO();
void SIMD2PICO();
void PICO2SIMD();
inline void PICO_Write();
inline void PICO_Read();

#endif
