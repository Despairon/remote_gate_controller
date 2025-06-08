#ifndef RECEIVER_IO_IFACE_IMPL_H
#define RECEIVER_IO_IFACE_IMPL_H

extern "C"
{
    #include <interfaces.h>
}

typedef struct generic_io_init_data_s
{
    iface_ctx_t uart_ctx;
    iface_ctx_t time_iface_ctx;
} generic_io_init_data_t;

extern const init_iface_t generic_io_init;

#endif