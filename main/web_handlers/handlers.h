#include "esp_http_server.h"
#include "test_handler.h"
#include "index_handler.h"

static const httpd_uri_t test_uri = {
    .uri       = "/test",
    .method    = HTTP_GET,
    .handler   = test_handler,
    .user_ctx  = NULL
};

static const httpd_uri_t index_uri = {
    .uri       = "/*",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
};