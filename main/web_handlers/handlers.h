#include "esp_http_server.h"
#include "index_handler.h"
#include "gpio_handler.h"

static const httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t include_uri = {
    .uri       = "/include/*",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t gpio_uri = {
    .uri       = "/gpio/*",
    .method    = HTTP_GET,
    .handler   = get_gpio_handler,
    .user_ctx  = NULL
};
