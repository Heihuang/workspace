#ifndef NGX_HTTP_ADPARK_MODULE_H
#define NGX_HTTP_ADPARK_MODULE_H
#include <ngx_config.h>
#include <nginx.h>
#include <ngx_http.h>

typedef struct {
    ngx_str_t   name;
    ngx_str_t   so_path;
    ngx_str_t   so_conf;
} ad_conf_t;

typedef struct {
    ngx_array_t plugin_conf;
} ngx_http_ad_main_conf_t;

#endif