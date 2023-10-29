/** 
 * @brief HTTP Server with Wi-Fi client
*/

#include <string.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <driver/gpio.h>
#include "esp_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>

#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>

#include "esp_netif.h"
#include "esp_eth.h"
#include "driver/gpio.h"
#include "esp_http_server.h"
#include "wifi.h"
#include "esp_vfs.h"
#include "dirent.h"
#include "esp_spiffs.h"
#include "http_server.h"
#include "web_handlers/handlers.h"

#define AP_WIFI_SSID CONFIG_WIFI_SSID
#define AP_WIFI_PASS CONFIG_WIFI_PASSWORD

static const char *TAG = "MAIN";

// static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
// {
//     httpd_handle_t* server = (httpd_handle_t*) arg;
//     if (*server == NULL) {
//         ESP_LOGI(LOGNAME, "Starting webserver");
//         *server = start_webserver();
//     }
// }

// static void disconnect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
// {
//     httpd_handle_t* server = (httpd_handle_t*) arg;
//     if (*server) {
//         ESP_LOGI(LOGNAME, "Stopping webserver");
//         stop_webserver(*server);
//         *server = NULL;
//     }
// }

void list_dir(char* path)
{
    DIR* dir = opendir(path);
    assert(dir != NULL);
    while (true) {
        struct dirent* pe = readdir(dir);
        if (pe) {
            ESP_LOGI("Spiffs file", "d_name=%s d_ino=%d d_type=%x", pe->d_name, pe->d_ino, pe->d_type);
        }
    }
    closedir(dir);
}

void init_spiffs() {
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 10,
        .format_if_mount_failed =true
    };

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)",esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total,&used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG,"Failed to get SPIFFS partition information (%s)",esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG,"Partition size: total: %d, used: %d", total, used);
    }

    // TODO debug
    // list_dir("/spiffs/");
}

void set_gpio_levels(void)
{
    for (uint8_t gpio_num = 1; gpio_num < 5; gpio_num++)
    {
        gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
        gpio_set_level(gpio_num, 0);
    }
}

void app_main(void)
{
    httpd_uri_t endpoints[] = {
        gpio_uri,
        include_uri,
        index_uri,
    };

    // ESP_ERROR_CHECK(nvs_flash_erase());
    ESP_ERROR_CHECK(nvs_flash_init());

    init_spiffs();
    set_gpio_levels();

    bool wifi_connected = wifi_init_station(
        AP_WIFI_SSID,
        AP_WIFI_PASS
    );

    if (wifi_connected) {
        ESP_LOGI(TAG, "wifi_init_sta finished. Connected = %d", wifi_connected);
        start_webserver((httpd_uri_t *) endpoints, sizeof(endpoints) / sizeof(httpd_uri_t));
    } else {
        ESP_LOGI(TAG, "wifi_init_sta failed to connect");
    }
}
