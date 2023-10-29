#include <sys/param.h>
#include <esp_log.h>
#include <esp_system.h>
#include <http_parser.h>
#include "esp_http_server.h"
#include "index_handler.h"

#define SEND_BUF_LEN 256
#define FILE_SYSTEM_ROOT "/spiffs"
#define URI_MAX 256
#define URI_PATH_MAX URI_MAX+sizeof(FILE_SYSTEM_ROOT)

#define IS_FILE_EXT(filename, ext) \
    (strcasecmp(&filename[strlen(filename) - sizeof(ext) + 1], ext) == 0)

static const char *LOGNAME = "Index";

void log_headers(httpd_req_t *request)
{
    char*  buf;
    size_t buf_len;

    buf_len = httpd_req_get_hdr_value_len(request, "User-Agent") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        /* Copy null terminated value string into buffer */
        if (httpd_req_get_hdr_value_str(request, "User-Agent", buf, buf_len) == ESP_OK) {
            ESP_LOGI(LOGNAME, "Found header => User-Agent: %s", buf);
        }
        free(buf);
    }
}

/* Set HTTP response content type according to file extension */
esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filename)
{
    if (IS_FILE_EXT(filename, ".js")) {
        return httpd_resp_set_type(req, "text/javascript");
    } else if (IS_FILE_EXT(filename, ".html")) {
        return httpd_resp_set_type(req, "text/html");
    } else if (IS_FILE_EXT(filename, ".css")) {
        return httpd_resp_set_type(req, "text/css");
    } else if (IS_FILE_EXT(filename, ".jpg")) {
        return httpd_resp_set_type(req, "image/jpeg");
    } else if (IS_FILE_EXT(filename, ".ico")) {
        return httpd_resp_set_type(req, "image/x-icon");
    }
    /* This is a limited set only */
    /* For any other type always set as plain text */
    return httpd_resp_set_type(req, "text/plain");
}

const char* get_uri(httpd_req_t *request)
{
    if (strcmp(request->uri, "/") == 0) {
        return "/index.html";
    } else {
        return request->uri;
    }
}

size_t send_file(httpd_req_t *request, const char* file_name)
{
    static char response[SEND_BUF_LEN];
    size_t readed = 0;
    size_t readedTotal = 0;

    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "404 Not found");
        ESP_LOGW(LOGNAME, "File not found %s", file_name);
        return 0;
    }

    do {
        readed = fread(&response, 1, SEND_BUF_LEN, file);
        httpd_resp_send_chunk(request, response, readed);
        readedTotal += readed;
    } while(readed == SEND_BUF_LEN);

    fclose(file);

    httpd_resp_send_chunk(request, NULL, 0);

    return readedTotal;
}

/** 
 * Handler: GET /
 */
esp_err_t index_handler(httpd_req_t *request)
{
    char file_name[URI_PATH_MAX] = FILE_SYSTEM_ROOT;

    log_headers(request);

    const char* method_name = http_method_str(request->method);
    const char* uri = get_uri(request);
    
    strlcat(file_name, uri, URI_PATH_MAX);

    set_content_type_from_file(request, file_name);
    size_t readed = send_file(request, file_name);

    if (readed != 0) {
        ESP_LOGI(LOGNAME, "%s %s (%d bytes)", method_name, uri, readed);
    }

    return ESP_OK;
}
