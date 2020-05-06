#include <nginx/ngx_core.h>

ngx_uint_t             ngx_test_config;

extern ngx_int_t ngx_save_argv(ngx_cycle_t *cycle, int argc, char *const *argv);


int main(int argc, char **argv)
{
    ngx_cycle_t *cycle, init_cycle;
    ngx_log_t   *log;
  
    ngx_debug_init();
  
    if (ngx_strerror_init() != NGX_OK) {
        return 1;
    }
  
    ngx_time_init();

    log = ngx_log_init((u_char *) ".");
    if (log == NULL) {
        return 1;
    }
  
    ngx_memzero(&init_cycle, sizeof(ngx_cycle_t));
    init_cycle.log = log;
    cycle = &init_cycle;
  
    if (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        return 1;
    }

    init_cycle.pool = ngx_create_pool(1024, log);
    if (init_cycle.pool == NULL) {
        return 1;
    }

    if (ngx_os_init(log) != NGX_OK) {
        return 1;
    }

    ngx_slab_sizes_init();

    if (ngx_preinit_modules() != NGX_OK) {
        return 1;
    }

    cycle->conf_file.len = strlen("nginx.conf");
    cycle->conf_file.data = (u_char *)"nginx.conf";

    cycle = ngx_init_cycle(&init_cycle);
    if (cycle == NULL) {
        if (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        return 1;
    }

    ngx_log_error(NGX_LOG_ERR, log, 0, "hello nginx log %d", 1);
    ngx_log_error(NGX_LOG_ERR, log, 2, "some error %s", "badarg");
    return 0;
}
