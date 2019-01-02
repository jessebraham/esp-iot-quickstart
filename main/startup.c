#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include "startup.h"


void initialize_device(EventGroupHandle_t *wifi_event_group)
{
    // Initialize the flash memory, erasing the entirety of its contents if
    // there are no free pages.
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    // Disable logging originating from within the wifi driver, initialize the
    // TCP/IP adapter, and create the wifi event group.
    esp_log_level_set("wifi", ESP_LOG_NONE);
    tcpip_adapter_init();
    *wifi_event_group = xEventGroupCreate();

    // Configure the wifi peripheral, setting the default configuration
    // generated by the ESP-IDF SDK as well as its default storage and
    // operating mode.
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
}
