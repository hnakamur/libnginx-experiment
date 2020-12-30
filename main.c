#include <libnginx.h>

int main(int argc, char **argv)
{
    int                  ret;
    ngx_cycle_t         *cycle;
    ngx_pool_t          *pool;
    void                *data;
    ngx_slab_pool_t     *sp;
    void                *p;
    ngx_shm_t            shm;

    ret = libnginx_init(".", "debug.log", NGX_LOG_DEBUG, 0);
    if (ret != NGX_OK) {
        fprintf(stderr, "libnginx initialize failed\n");
        return 1;
    }
    cycle = (ngx_cycle_t *) ngx_cycle;

    ngx_log_error(NGX_LOG_ERR, cycle->log, 0, "hello nginx log %d", 1);
    ngx_log_error(NGX_LOG_ERR, cycle->log, 2, "some error %s", "badarg");

    sleep(1);
    ngx_time_update();
    ngx_log_debug0(NGX_LOG_DEBUG, cycle->log, 0, "This is debug a log");
    ngx_log_debug1(NGX_LOG_DEBUG, cycle->log, 0, "This is debug a log, a=%d", 1);

    /* ==============================
     * memory pool usage example
     * ============================== */
    /* NOTE: The minimum size is 80.
     * With size <= 79, ngx_pfree returns -NGX_DECLINE. */
    pool = ngx_create_pool(80, cycle->log);
    data = ngx_pcalloc(pool, 80);
    ngx_log_debug1(NGX_LOG_DEBUG, cycle->log, 0, "data=%p", data);
    ret = ngx_pfree(pool, data);
    ngx_log_debug1(NGX_LOG_DEBUG, cycle->log, 0, "ngx_pfree ret=%d", ret);
    ngx_destroy_pool(pool);

    /* ==============================
     * slab pool usage example
     * ============================== */
    memset(&shm, 0, sizeof(shm));
    shm.size = 1024 * 1024;; /* 1MiB */
    ngx_str_set(&shm.name, "libnginx_shared_zone");
    shm.log = cycle->log;

    if (ngx_shm_alloc(&shm) != NGX_OK) {
        return NGX_ERROR;
    }
    ngx_log_debug1(NGX_LOG_DEBUG, cycle->log, 0, "shm.addr=%p", shm.addr);
    if (libnginx_init_shm_pool(cycle, &shm) != NGX_OK) {
        fprintf(stderr, "libnginx_init_shm_pool failed\n");
        return NGX_ERROR;
    }
    sp = (ngx_slab_pool_t *) shm.addr;
    p = ngx_slab_calloc(sp, 20);
    ngx_log_debug1(NGX_LOG_DEBUG, cycle->log, 0, "slab_alloc p=%p", p);
    ngx_slab_free(sp, p);
    ngx_shm_free(&shm);

    /* ==============================
     * shared memory example
     * ============================== */
    shm.size = 1024 * 1024;

    return 0;
}
