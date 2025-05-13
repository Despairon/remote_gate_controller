#ifndef RGC_GENERIC_IO_IFACE_IMPL_H
#define RGC_GENERIC_IO_IFACE_IMPL_H

extern "C"
{
    #include <interfaces.h>
}

typedef struct generic_io_iface_init_data_s
{
    generic_iface_handle_t serial_iface_handle;
    generic_iface_handle_t time_iface_handle;
} generic_io_iface_init_data_t;

extern const generic_iface_ctor_dtor_t generic_io_iface_ctor_dtor;

#endif