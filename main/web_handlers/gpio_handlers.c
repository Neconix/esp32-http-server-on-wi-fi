#include <sys/param.h>
#include <esp_log.h>
#include <esp_system.h>
#include <http_parser.h>
#include "esp_http_server.h"
#include "driver/gpio.h"
#include "index_handler.h"
#include "cJSON.h"

#define HTTP_GET 1

#define IS_FILE_EXT(filename, ext) \
    (strcasecmp(&filename[strlen(filename) - sizeof(ext) + 1], ext) == 0)

static const char *LOGNAME = "Gpio handler";

gpio_num_t get_gpio_num(httpd_req_t *request)
{
    int num = 0;

    sscanf(request->uri, "/gpio/%d", &num);

    return num;
}

uint8_t get_gpio_value(gpio_num_t gpio_num)
{
    return gpio_get_level(gpio_num);
}

void send_response(httpd_req_t *request, u_int8_t gpio_num, u_int8_t gpio_value)
{
    const char response[100];

    httpd_resp_set_type(request, "application/json");

    const char* response_template = "{ \"gpio\": %d, \"value\": %d }";
    sprintf(
        response, 
        response_template, 
        gpio_num,
        gpio_value
    );

    httpd_resp_send(request, response, strlen(response));
}

/** 
 * Handler: GET /gpio/
 */
esp_err_t get_gpio_handler(httpd_req_t *request)
{
    const char* method_name = http_method_str(request->method);

    ESP_LOGI(LOGNAME, "%s %s", method_name, request->uri);

    gpio_num_t gpio_num = get_gpio_num(request);
    uint8_t gpio_value = get_gpio_value(gpio_num);

    send_response(request, gpio_num, gpio_value);

    return ESP_OK;
}

int parse_gpio_level(httpd_req_t *request)
{
    int total_len = request->content_len;
    int cur_len = 0;
    int received = 0;
    size_t buflen = 256;

    char *buf = malloc(buflen + 1);
    if (total_len >= buflen) {
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len) {
        received = httpd_req_recv(request, buf + cur_len, total_len);
        if (received <= 0) {
            httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';

    cJSON *root = cJSON_Parse(buf);
    int level = cJSON_GetObjectItem(root, "level")->valueint;

    cJSON_Delete(root);

    free(buf);

    return level;
}

/** 
 * Handler: PUT /gpio/
 */
esp_err_t put_gpio_handler(httpd_req_t *request)
{
    const char* method_name = http_method_str(request->method);

    ESP_LOGI(LOGNAME, "%s %s", method_name, request->uri);

    gpio_num_t gpio_num = get_gpio_num(request);
    uint8_t gpio_value = get_gpio_value(gpio_num);

    int level = parse_gpio_level(request);

    ESP_LOGI(LOGNAME, "Set level %d", level);

    gpio_set_level(gpio_num, level);

    httpd_resp_sendstr(request, "OK");

    return ESP_OK;
}
