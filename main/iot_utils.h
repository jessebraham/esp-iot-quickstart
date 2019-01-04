#ifndef IOT_UTILS_H
#define IOT_UTILS_H

#include "esp_system.h"
#include "mqtt_client.h"


void initialize_device(void);
void wifi_connect(const char *ssid, const char *pass);

void connect_to_mqtt_broker(
    const char *uri, esp_err_t (* fn)(esp_mqtt_event_handle_t)
);

#endif
