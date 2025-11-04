#include <stdio.h>
#include "esp_spiffs.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"

#include "protocol_examples_common.h"
#include "esp_wifi.h"

#define wifiTAG "simple_connect_example"
static const char *TAG = "FileSystem";

void app_main(void)
{
    // connect to wifi
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(example_connect());

    // Get the ESP32's IP address
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif == NULL)
    {
        ESP_LOGE(wifiTAG, "Failed to get esp_netif handle");
    }
    else
    {
        esp_netif_ip_info_t ip_info;
        ESP_ERROR_CHECK(esp_netif_get_ip_info(netif, &ip_info));
        ESP_LOGI(wifiTAG, "ESP32 IP Address: " IPSTR, IP2STR(&ip_info.ip));
        ESP_LOGI(wifiTAG, "ESP32 Netmask: " IPSTR, IP2STR(&ip_info.netmask));
        ESP_LOGI(wifiTAG, "ESP32 Gateway: " IPSTR, IP2STR(&ip_info.gw));
    }

    // // Print out Access Point Information
    // wifi_ap_record_t ap_info;
    // ESP_ERROR_CHECK(esp_wifi_sta_get_ap_info(&ap_info));
    // ESP_LOGI(wifiTAG, "--- Access Point Information ---");
    // ESP_LOG_BUFFER_HEX("MAC Address", ap_info.bssid, sizeof(ap_info.bssid));
    // ESP_LOG_BUFFER_CHAR("SSID", ap_info.ssid, sizeof(ap_info.ssid));
    // ESP_LOGI(wifiTAG, "Primary Channel: %d", ap_info.primary);
    // ESP_LOGI(wifiTAG, "RSSI: %d", ap_info.rssi);

    // mount file system
    esp_vfs_spiffs_conf_t config = {
        .base_path = "/storage",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};

    esp_err_t result = esp_vfs_spiffs_register(&config);
    if (result != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initiialize SPIFFS(%s)", esp_err_to_name(result));
        return;
    }

    size_t total = 0, used = 0;
    result = esp_spiffs_info(config.partition_label, &total, &used);
    if (result != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to to get partition (%s)", esp_err_to_name(result));
        return;
    }
    else
    {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    FILE *book = fopen("/storage/frankenstein.txt", "r");

    char line[64];
    fgets(line, sizeof(line), book);
    fclose(book);
    printf("%s\n", line);
}
