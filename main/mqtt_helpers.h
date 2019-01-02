#ifndef MQTT_HELPERS_H
#define MQTT_HELPERS_H

#include "esp_system.h"
#include "mqtt_client.h"


void connect_to_mqtt_broker(
    const char *uri, esp_err_t (* fn)(esp_mqtt_event_handle_t)
);

#endif
