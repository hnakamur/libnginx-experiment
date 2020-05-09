#include <nginx/ngx_core.h>

int main(int argc, char **argv)
{
    ngx_log_t   *log;
  
    ngx_debug_init();
  
    if (ngx_strerror_init() != NGX_OK) {
        return 1;
    }
  
    ngx_time_init();

    log = ngx_log_init_name((u_char *) "debug.log");
    if (log == NULL) {
        return 1;
    }
    log->log_level = NGX_LOG_DEBUG;
    ngx_use_stderr = 0;

    ngx_log_error(NGX_LOG_ERR, log, 0, "hello nginx log %d", 1);
    ngx_log_error(NGX_LOG_ERR, log, 2, "some error %s", "badarg");

    sleep(1);
    ngx_time_update();
    ngx_log_debug0(NGX_LOG_DEBUG, log, 0, "This is debug a log");
    ngx_log_debug1(NGX_LOG_DEBUG, log, 0, "This is debug a log, a=%d", 1);
    return 0;
}
