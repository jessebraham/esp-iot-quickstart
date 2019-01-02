#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"

#include "sdkconfig.h"

#include "startup.h"


///////////////////////////////////////////////////////////////////////////////
// CONSTANTS & GLOBALS

// logging
static const char *TAG = "ESP-IOT-QUICKSTART";

// wifi
const  int                CONNECTED_BIT    = BIT0;
static EventGroupHandle_t wifi_event_group = NULL;


///////////////////////////////////////////////////////////////////////////////
// EVENT HANDLERS

void wifi_connect(void)
{
    wifi_config_t sta_config = {
        .sta = {
            .ssid      = CONFIG_ESP_WIFI_SSID,
            .password  = CONFIG_ESP_WIFI_PASSWORD,
            .bssid_set = false
        }
    };
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );
}

esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
    case SYSTEM_EVENT_STA_START:
        wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
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
    initialize_device(&wifi_event_group);
    ESP_LOGI(TAG, "Device peripherals initialized");

    // set the event loop handler for wifi and start the wifi peripheral
    ESP_ERROR_CHECK( esp_event_loop_init(wifi_event_handler, NULL) );
    ESP_LOGI(TAG, "Starting wifi adapter...");
    ESP_ERROR_CHECK( esp_wifi_start() );
}
