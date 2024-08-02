#ifndef RPICO_H
#define RPICO_H
#endif

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

void pinMode(int, int);
int digitalRead(int);
void digitalWrite(int , int);
void analogWrite(int ,int,int);
uint16_t analogRead(int);