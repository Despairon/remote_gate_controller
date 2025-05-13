#include <remote_gate_control/generic_time_iface_impl.h>
#include <Arduino.h>

typedef struct generic_time_iface_data_s
{
    const generic_time_iface_t *iface;
} generic_time_iface_data_t;

static generic_iface_handle_t create_generic_time_iface(void *init_data);
static void destroy_generic_time_iface(generic_iface_handle_t handle);

static uint64_t generic_time_get_time_ms(generic_iface_handle_t handle);

const generic_iface_ctor_dtor_t generic_time_iface_ctor_dtor =
{
    .create  = &create_generic_time_iface,
    .destroy = &destroy_generic_time_iface
};

static const generic_time_iface_t generic_time_iface =
{
    .get_time_ms = &generic_time_get_time_ms
};

static generic_iface_handle_t create_generic_time_iface(void *init_data)
{
    (void)init_data;
    
    generic_iface_handle_t handle = NULL;

    generic_time_iface_data_t *iface_data = (generic_time_iface_data_t*)malloc(sizeof(generic_time_iface_data_t));
    if (iface_data)
    {
       iface_data->iface = &generic_time_iface;

       handle = (generic_iface_handle_t)iface_data;
    }

    return handle;
}

static void destroy_generic_time_iface(generic_iface_handle_t handle)
{
    if (handle)
        free((generic_time_iface_data_t*)handle);
}

static uint64_t generic_time_get_time_ms(generic_iface_handle_t handle)
{
    (void)handle;

    return millis();
}