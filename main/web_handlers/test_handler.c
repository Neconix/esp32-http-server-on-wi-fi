#include <sys/param.h>
#include <esp_log.h>
#include <esp_system.h>
#include "esp_http_server.h"
#include "test_handler.h"

static const char *LOGNAME = "Root handler";

/* Handler: GET /test */
esp_err_t test_handler(httpd_req_t *req)
{
    char*  buf;
    size_t buf_len;

    /* Get header value string length and allocate memory for length + 1,
     * extra byte for null termination */
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        /* Copy null terminated value string into buffer */
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
            ESP_LOGI(LOGNAME, "Found header => Host: %s", buf);
        }
        free(buf);
    }

    /* Send response with custom headers and body set as the
     * string passed in user context*/
    // const char* resp_str = (const char*) req->user_ctx;
    static char resp_str[] = "Test server string";
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

    /* After sending the HTTP response the old HTTP request
     * headers are lost. Check if HTTP request headers can be read now. */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI(LOGNAME, "Request headers lost");
    }
    return ESP_OK;
}
