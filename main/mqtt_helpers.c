#include "mqtt_helpers.h"


void connect_to_mqtt_broker(
    const char *uri, esp_err_t (* fn)(esp_mqtt_event_handle_t)
)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .uri          = uri,
        .event_handle = fn,
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);
}
