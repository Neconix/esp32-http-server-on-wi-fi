#include <sys/param.h>
#include <esp_log.h>
#include <esp_system.h>
#include "esp_http_server.h"
#include "http_server.h"

static const char *LOGNAME = "httpd";

httpd_handle_t start_webserver(httpd_uri_t* endpoints, size_t count)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;
    config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_LOGI(LOGNAME, "Starting httpd on port: %d", config.server_port);
    if (httpd_start(&server, &config) != ESP_OK) {
        ESP_LOGI(LOGNAME, "Error starting httpd");

        return NULL;
    }

    
    ESP_LOGI(LOGNAME, "Registering URI handlers");

    for (size_t i = 0; i < count; i++)
    {
        httpd_uri_t* endpoint = &endpoints[i];
        ESP_LOGI(LOGNAME, "Register endpoint %s", endpoint->uri);
        httpd_register_uri_handler(server, endpoint);
    }
        
    return server;
}

static void stop_webserver(httpd_handle_t server)
{
    httpd_stop(server);
}
