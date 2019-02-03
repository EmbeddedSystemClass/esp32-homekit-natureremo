#ifndef __SYSTEMLED_H__
#define __SYSTEMLED_H__

#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "driver/gpio.h"

typedef struct {
    int gpio;
    bool on;
} led_config_t;

void systemled_init();
void systemled_on();
void systemled_off();
void systemled_blink();
bool systemled_status();

#endif
