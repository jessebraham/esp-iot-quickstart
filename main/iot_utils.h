#ifndef IOT_UTILS_H
#define IOT_UTILS_H

#include "esp_system.h"
#include "mqtt_client.h"


/*
 * Perform some basic setup of the device prior to running our actual
 * application. Initializes and, if necessary, clears the Flash memory, and
 * configures the wifi peripheral for use in STA mode.
 */
void initialize_device(void);

/*
 * Given an SSID and password, attempt to connect to the wifi network.
 */
void connect_to_wifi_network(const char *ssid, const char *pass);

/*
 * Connect to an MQTT broker and register the handler for its event loop. The
 * provided URI may optionally include credentials for authenticating with the
 * broker.
 * 
 *   eg.) user:pass@127.0.0.1:1883
 *        mqtt.broker.local:1883
 */
void connect_to_mqtt_broker(
    const char *uri, esp_err_t (* fn)(esp_mqtt_event_handle_t)
);

#endif
