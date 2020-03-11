#include <assert.h>
#include "utils.h"
#include "ngx_http_adpark_module.h"

static void *ngx_http_adpark_create_main_conf(ngx_conf_t *cf);
static char *ngx_http_adpark_init_main_conf(ngx_conf_t *cf, void *conf);
static char *ngx_http_adpark_set_conf(ngx_conf_t *cf, ngx_command_t *cmd, void *conf); 

static ngx_int_t ngx_http_adpark_init_module(ngx_cycle_t *cycle);
static ngx_int_t ngx_http_adpark_init_process(ngx_cycle_t *cycle);
static void ngx_http_adpark_exit_process(ngx_cycle_t *cycle);
static void ngx_http_adpark_exit_master(ngx_cycle_t *cycle);

static ngx_command_t  ngx_http_adpark_commands[] = {

    { ngx_string("adpark_conf_path"),
        NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_NOARGS,
        ngx_http_adpark_set_conf, //将配置文件中相关指令的参数转化成需要的格式并存入配置结构体
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL },

    ngx_null_command
};

static ngx_http_module_t  ngx_http_adpark_module_ctx = {
    NULL,                                   /* preconfiguration */
    NULL,                                   /* postconfiguration */

    ngx_http_adpark_create_main_conf,       /* create main configuration */
    ngx_http_adpark_init_main_conf,         /* init main configuration */

    NULL,                                   /* create server configuration */
    NULL,                                   /* merge server configuration */

    NULL,                                   /* create location configuration */
    NULL                                    /* merge location configuration */
};

ngx_module_t  ngx_http_adpark_module = {
    NGX_MODULE_V1,
    &ngx_http_adpark_module_ctx,            /* module context */
    ngx_http_adpark_commands,               /* module directives */
    NGX_HTTP_MODULE,                        /* module type */
    NULL,                                   /* init master */
    ngx_http_adpark_init_module,            /* init module */
    ngx_http_adpark_init_process,           /* init process */
    NULL,                                   /* init thread */
    NULL,                                   /* exit thread */
    ngx_http_adpark_exit_process,           /* exit process */
    ngx_http_adpark_exit_master,            /* exit master */
    NGX_MODULE_V1_PADDING
};

static void *ngx_http_adpark_create_main_conf(ngx_conf_t *cf) {
    ngx_http_adpark_main_conf_t *conf;
    
    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_adpark_main_conf_t)); 
    if(conf == NULL) {
        return NGX_CONF_ERROR;
    }

    return conf;
}

static char *ngx_http_adpark_init_main_conf(ngx_conf_t *cf, void *conf) {
    return NGX_CONF_OK; 
}


static char *ngx_http_adpark_set_conf(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_str_t *value;
    adpark_conf_t *pcf;
    ngx_array_t *adpark_conf = &((ngx_http_adpark_main_conf_t *)conf)->adpark_conf;

    if(adpark_conf->elts == NULL) {
        if(ngx_array_init(adpark_conf, cf->pool, 1, 
                    sizeof(adpark_conf_t)) != NGX_OK) {
            return NGX_CONF_ERROR;
        }
    }

    assert(cf->args->nelts == 4);

    pcf = ngx_array_push(adpark_conf);
    value = cf->args->elts;

    /* name */
    if(ngx_strncmp(value[1].data, "name=", 5) != 0) {
        ngx_log_error(NGX_LOG_ERR, cf->log, 0, 
                "[adpark] invalid adpark_conf %V", value + 1);

        return NGX_CONF_ERROR;
    }

    pcf->name.data = value[1].data + 5;
    pcf->name.len = value[1].len - 5;

    /* so_path */
    if(ngx_strncmp(value[2].data, "so_path=", 8) != 0) {
        ngx_log_error(NGX_LOG_ERR, cf->log, 0, 
                "[adpark] invalid adpark_conf %V", value + 2);

        return NGX_CONF_ERROR;
    }

    pcf->so_path.data = value[2].data + 8;
    pcf->so_path.len = value[2].len - 8;

    /* so_conf */
    if(ngx_strncmp(value[3].data, "so_conf=", 8) != 0) {
        ngx_log_error(NGX_LOG_ERR, cf->log, 0, 
                "[adpark] invalid adpark_conf %V", value + 3);

        return NGX_CONF_ERROR;
    }

    pcf->so_conf.data = value[3].data + 8;
    pcf->so_conf.len = value[3].len - 8;

    return NGX_CONF_OK; 
}


static ngx_int_t ngx_http_adpark_init_module(ngx_cycle_t *cycle) {
    
    ngx_http_adpark_main_conf_t *conf;
    conf = ngx_http_cycle_get_module_main_conf(cycle, ngx_http_adpark_module);

    ngx_log_error(NGX_LOG_DEBUG, cycle->log, 0, "[adpark] create adpark manager start");

    if(adpark_create_manager(conf) != NGX_OK) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0, "[adpark] create adpark manager fail");
        return NGX_ERROR;   
    }

    if(adpark_init_master() != NGX_OK) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0, "[adpark] adpark init module fail");
        return NGX_ERROR;   
    }

    ngx_log_error(NGX_LOG_DEBUG, cycle->log, 0, "[adpark] create adpark manager success");
    

    return NGX_OK;
}


static ngx_int_t ngx_http_adpark_init_process(ngx_cycle_t *cycle) {
    if(adpark_init_process() != NGX_OK) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0, "[adpark] adpark init process fail");
        return NGX_ERROR;   
    }

    return NGX_OK;
}


static void ngx_http_adpark_exit_process(ngx_cycle_t *cycle) {
    ngx_log_error(NGX_LOG_DEBUG, cycle->log, 0, "[adpark] exit process");

    adpark_exit_process();
}


static void ngx_http_adpark_exit_master(ngx_cycle_t *cycle) {
    ngx_log_error(NGX_LOG_DEBUG, cycle->log, 0, "[adpark] exit master");

    adpark_exit_master();
}