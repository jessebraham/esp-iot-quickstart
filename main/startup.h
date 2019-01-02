#ifndef STARTUP_H
#define STARTUP_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"


void initialize_device(EventGroupHandle_t *wifi_event_group);

#endif
