#ifndef GPIO_IFACE_IMPL_H
#define GPIO_IFACE_IMPL_H

extern "C"
{
    #include <interfaces.h>
}

typedef struct gpio_iface_init_data_s
{
    int pin_num;
} gpio_iface_init_data_t;

typedef uint32_t gpio_state_t;

extern const interface_t gpio_iface;

#endif