#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "sdkconfig.h"

#include "mqtt_helpers.h"
#include "startup.h"


///////////////////////////////////////////////////////////////////////////////
// CONSTANTS & GLOBALS

// logging
static const char *TAG = CONFIG_LOGGING_TAG;

// wifi
const  int                CONNECTED_BIT    = BIT0;
static EventGroupHandle_t wifi_event_group = NULL;


///////////////////////////////////////////////////////////////////////////////
// EVENT HANDLERS

esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "connected to mqtt broker");
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "disconnected from mqtt broker");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        break;
    case MQTT_EVENT_PUBLISHED:
        break;
    case MQTT_EVENT_DATA:
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "mqtt error occurred");
        break;
    default:
        break;
    }

    return ESP_OK;
}

esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
    case SYSTEM_EVENT_STA_START:
        ESP_LOGI(TAG, "wifi adapter started");
        wifi_connect(CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD);
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "received ip address");
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        connect_to_mqtt_broker(CONFIG_MQTT_BROKER_URI, &mqtt_event_handler);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        ESP_LOGI(TAG, "disconnected from wireless network");
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }

    return ESP_OK;
}


///////////////////////////////////////////////////////////////////////////////
// MAIN

void app_main(void)
{
    // perform any necessary initialization/configuration of the device and its
    // peripherals
    ESP_LOGI(TAG, "initializing device peripherals...");
    initialize_device(&wifi_event_group);

    // set the event loop handler for wifi and start the wifi peripheral
    ESP_ERROR_CHECK( esp_event_loop_init(wifi_event_handler, NULL) );
    ESP_LOGI(TAG, "starting wifi adapter...");
    ESP_ERROR_CHECK( esp_wifi_start() );
}
