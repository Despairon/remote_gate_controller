#include <remote_gate_control/time_iface_impl.h>
#include <Arduino.h>

typedef struct time_iface_data_s
{
    const time_iface_t *iface;
} time_iface_data_t;

static iface_ctx_t create_time_iface(void *init_data);
static void destroy_time_iface(iface_ctx_t ctx);

static uint64_t generic_time_get_time_ms(iface_ctx_t ctx);

const init_iface_t time_iface_init =
{
    .create  = &create_time_iface,
    .destroy = &destroy_time_iface
};

static const time_iface_t time_iface =
{
    .get_time_ms = &generic_time_get_time_ms
};

static iface_ctx_t create_time_iface(void *init_data)
{
    (void)init_data;
    
    iface_ctx_t ctx = NULL;

    time_iface_data_t *iface_data = (time_iface_data_t*)malloc(sizeof(time_iface_data_t));
    if (iface_data)
    {
       iface_data->iface = &time_iface;

       ctx = (iface_ctx_t)iface_data;
    }

    return ctx;
}

static void destroy_time_iface(iface_ctx_t ctx)
{
    if (ctx)
        free((time_iface_data_t*)ctx);
}

static uint64_t generic_time_get_time_ms(iface_ctx_t ctx)
{
    (void)ctx;

    return millis();
}