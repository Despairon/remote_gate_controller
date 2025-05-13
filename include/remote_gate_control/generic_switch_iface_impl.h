#ifndef RGC_GENERIC_SWITCH_IFACE_IMPL_H
#define RGC_GENERIC_SWITCH_IFACE_IMPL_H

extern "C"
{
    #include <interfaces.h>
}

typedef struct generic_switch_iface_init_data_s
{
    int pin_num;
} generic_switch_iface_init_data_t;

extern const generic_iface_ctor_dtor_t generic_switch_iface_ctor_dtor;

#endif