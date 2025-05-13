extern "C"
{
    // TODO: add esp8266 driver and implement the interface according to its functions
    #include <esp8266/include/esp8266_driver.h>
}

#include <remote_gate_control/generic_io_iface_impl.h>
#include <Arduino.h>

typedef struct generic_io_iface_data_s
{
    const generic_io_iface_t *iface;
    generic_io_iface_init_data_t init_data;
} generic_io_iface_data_t;

static generic_iface_handle_t create_generic_io(void *init_data);
static void destroy_generic_io(generic_iface_handle_t handle);

static size_t generic_io_send_data(generic_iface_handle_t handle, buffer_t *buffer);
static size_t generic_io_receive_data(generic_iface_handle_t handle, buffer_t *buffer);
static size_t generic_io_poll_available_data(generic_iface_handle_t handle);

const generic_iface_ctor_dtor_t generic_io_iface_ctor_dtor =
{
    .create  = &create_generic_io,
    .destroy = &destroy_generic_io
};

static const generic_io_iface_t generic_io_iface =
{
    .send_data           = &generic_io_send_data,
    .receive_data        = &generic_io_receive_data,
    .poll_available_data = &generic_io_poll_available_data
};

static generic_iface_handle_t create_generic_io(void *init_data)
{
    generic_iface_handle_t handle = NULL;

    if (init_data)
    {
        generic_io_iface_data_t *iface_data = (generic_io_iface_data_t*)malloc(sizeof(generic_io_iface_data_t));
        if (iface_data)
        {
            iface_data->iface = &generic_io_iface;
            
            (void)memcpy(&(iface_data->init_data), init_data, sizeof(generic_io_iface_init_data_t));

            handle = (generic_iface_handle_t)iface_data;
        }
    }

    return handle;
}

static void destroy_generic_io(generic_iface_handle_t handle)
{
    if (handle)
        free((generic_io_iface_data_t*)handle);
}

static size_t generic_io_send_data(generic_iface_handle_t handle, buffer_t *buffer)
{
    // TODO: implement using esp8266 driver
    return 0;
}

static size_t generic_io_receive_data(generic_iface_handle_t handle, buffer_t *buffer)
{
    // TODO: implement using esp8266 driver
    return 0;
}

static size_t generic_io_poll_available_data(generic_iface_handle_t handle)
{
    // TODO: implement using esp8266 driver
    // TODO: consider using "esp8266.tick() here!"
    return 0;
}