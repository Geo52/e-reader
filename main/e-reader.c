#include <stdio.h>
#include "esp_spiffs.h"
#include "esp_log.h"

static const char *TAG = "FileSystem";

void app_main(void)
{
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
