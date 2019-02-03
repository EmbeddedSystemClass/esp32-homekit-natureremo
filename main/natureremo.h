#ifndef __NATUREREMO_H__
#define __NATUREREMO_H__

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "wifi_setup.h"

#include "esp_http_client.h"

typedef struct {
    char* name;
    char* value;
} natureremo_signal_t;

typedef struct {
    char* name;
    bool on;    
    natureremo_signal_t signals[2];
} natureremo_ceiling_light_t;

void natureremo_init();
void natureremo_ceiliing_light_on(int light_no);
void natureremo_ceiliing_light_off(int light_no);
bool natureremo_ceiliing_light_status(int light_no);

#endif
