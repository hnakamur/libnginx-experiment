#include <nginx/ngx_core.h>

int main(int argc, char **argv)
{
    ngx_log_t *log;
    int ret = libnginx_init(&log, "debug.log", NGX_LOG_DEBUG, 0);

    ngx_log_error(NGX_LOG_ERR, log, 0, "hello nginx log %d", 1);
    ngx_log_error(NGX_LOG_ERR, log, 2, "some error %s", "badarg");

    sleep(1);
    ngx_time_update();
    ngx_log_debug0(NGX_LOG_DEBUG, log, 0, "This is debug a log");
    ngx_log_debug1(NGX_LOG_DEBUG, log, 0, "This is debug a log, a=%d", 1);

    /* ==============================
     * memory pool usage example
     * ============================== */
    /* NOTE: The minimum size is 80.
     * With size <= 79, ngx_pfree returns -NGX_DECLINE. */
    ngx_pool_t *pool = ngx_create_pool(80, log);
    void *data = ngx_pcalloc(pool, 80);
    ngx_log_debug1(NGX_LOG_DEBUG, log, 0, "data=%p", data);
    ret = ngx_pfree(pool, data);
    ngx_log_debug1(NGX_LOG_DEBUG, log, 0, "ngx_pfree ret=%d", ret);
    ngx_destroy_pool(pool);

    /* ==============================
     * slab pool usage example
     * ============================== */
    const size_t pool_size = 1024 * 1024; /* 1MiB */
    /* NOTE: A shared memory is used in nginx for backstore of slab_pool,
     * but we use an unshared memory here since we use only one process. */
    ngx_slab_pool_t *sp = malloc(pool_size);
    if (sp == NULL) {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }
    if (libnginx_slab_init_size(sp, pool_size) != NGX_OK) {
        fprintf(stderr, "ngx_slab_init_size failed\n");
        return NGX_ERROR;
    }

    void *p = ngx_slab_calloc(sp, 20);
    ngx_log_debug1(NGX_LOG_DEBUG, log, 0, "slab_alloc p=%p", p);
    ngx_slab_free(sp, p);
    free(sp);

    return 0;
}
