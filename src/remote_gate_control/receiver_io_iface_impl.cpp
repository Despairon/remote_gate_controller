extern "C"
{
    // TODO: add esp8266 driver and implement the interface according to its functions
    #include <esp8266/include/esp8266_driver.h>
}

#include <remote_gate_control/receiver_io_iface_impl.h>
#include <Arduino.h>

typedef struct generic_io_data_s
{
    const generic_io_t *iface;
    generic_io_init_data_t init_data;
} generic_io_data_t;

static iface_ctx_t create_generic_io(void *init_data);
static void destroy_generic_io(iface_ctx_t ctx);

static size_t generic_io_send_data(iface_ctx_t ctx, buffer_t *buffer);
static size_t generic_io_receive_data(iface_ctx_t ctx, buffer_t *buffer);
static size_t generic_io_available_data(iface_ctx_t ctx);

const init_iface_t generic_io_init =
{
    .create  = &create_generic_io,
    .destroy = &destroy_generic_io
};

static const generic_io_t generic_io =
{
    .send_data           = &generic_io_send_data,
    .receive_data        = &generic_io_receive_data,
    .available_data = &generic_io_available_data
};

static iface_ctx_t create_generic_io(void *init_data)
{
    iface_ctx_t ctx = NULL;

    if (init_data)
    {
        generic_io_data_t *iface_data = (generic_io_data_t*)malloc(sizeof(generic_io_data_t));
        if (iface_data)
        {
            iface_data->iface = &generic_io;
            
            (void)memcpy(&(iface_data->init_data), init_data, sizeof(generic_io_init_data_t));

            ctx = (iface_ctx_t)iface_data;
        }
    }

    return ctx;
}

static void destroy_generic_io(iface_ctx_t ctx)
{
    if (ctx)
        free((generic_io_data_t*)ctx);
}

static size_t generic_io_send_data(iface_ctx_t ctx, buffer_t *buffer)
{
    // TODO: implement using esp8266 driver
    return 0;
}

static size_t generic_io_receive_data(iface_ctx_t ctx, buffer_t *buffer)
{
    // TODO: implement using esp8266 driver
    return 0;
}

static size_t generic_io_available_data(iface_ctx_t ctx)
{
    // TODO: implement using esp8266 driver
    // TODO: consider using "esp8266.tick() here!"
    return 0;
}