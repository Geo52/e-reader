#include <stdio.h>
#include "esp_spiffs.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_http_server.h"
#include "protocol_examples_common.h"
#include "esp_wifi.h"

#define wifiTAG "simple_connect_example"
static const char *TAG = "FileSystem";

extern const unsigned char homepage_html_start[] asm("_binary_homepage_html_start");
extern const unsigned char homepage_html_end[] asm("_binary_homepage_html_end");

static esp_err_t homepage_handler(httpd_req_t *req)
{
    size_t homepage_html_size = (homepage_html_end - homepage_html_start);

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)homepage_html_start, homepage_html_size);
    return ESP_OK;
}

static const httpd_uri_t homepage = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = homepage_handler,
};

void mount_filesystem()
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
}

void start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &homepage);
        // httpd_register_uri_handler(server, &upload_book);
        ESP_LOGI("webserver", "Server started on port %d", config.server_port);
    }
    else
    {
        ESP_LOGE("webserver", "Failed to start server");
    }
}

void app_main(void)
{
    // connect to wifi
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(example_connect());

    mount_filesystem();

    start_webserver();
}
