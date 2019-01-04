#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "sdkconfig.h"

#include "iot_utils.h"


///////////////////////////////////////////////////////////////////////////////
// CONSTANTS & GLOBALS

// logging
static const char *TAG = CONFIG_LOGGING_TAG;

// wifi & mqtt
const  int                CONNECTED_BIT    = BIT0;
static EventGroupHandle_t mqtt_event_group = NULL;
static EventGroupHandle_t wifi_event_group = NULL;


///////////////////////////////////////////////////////////////////////////////
// EVENT HANDLERS

esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "connected to mqtt broker");
        xEventGroupSetBits(mqtt_event_group, CONNECTED_BIT);
        //
        // Once a connection has been established to the MQTT broker, you will
        // likely want to subscribe to one or more topics here.
        //
        // eg.) msg_id = esp_mqtt_client_subscribe(client, "/topic/abc", 0);
        //      ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        //
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "disconnected from mqtt broker");
        xEventGroupClearBits(mqtt_event_group, CONNECTED_BIT);
        //
        // If any long-running routines that publish to the broker are running,
        // now would be a good time to terminate them.
        //
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "subscribed to topic, msg_id=%d", event->msg_id);
        //
        // Once a topic has been subscribed to, you can publish to that topic,
        // or simply wait for events to occur.
        //
        // eg.) msg_id = esp_mqtt_client_publish(
        //          client, "/topic/abc", "data", 0, 0, 0
        //      );
        //      ESP_LOGI(TAG, "send publish successful, msg_id=%d", msg_id);
        //
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "unsubscribed from topic, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "published to topic, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "data received from topic");
        ESP_LOGI(TAG, "topic=%.*s\r\n", event->topic_len, event->topic);
        ESP_LOGI(TAG, "data=%.*s\r\n", event->data_len, event->data);
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
        // once we have an ip address, attempt to connect to the mqtt broker
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
    // create the event groups for the mqtt component and the wifi peripheral
    mqtt_event_group = xEventGroupCreate();
    wifi_event_group = xEventGroupCreate();

    // perform any necessary initialization/configuration of the device and its
    // peripherals
    ESP_LOGI(TAG, "initializing device peripherals...");
    initialize_device();

    // set the event loop handler for wifi and start the wifi peripheral
    ESP_ERROR_CHECK( esp_event_loop_init(wifi_event_handler, NULL) );
    ESP_LOGI(TAG, "starting wifi adapter...");
    ESP_ERROR_CHECK( esp_wifi_start() );
}
