#ifndef RGC_PLATFORM_IFACES_H
#define RGC_PLATFORM_IFACES_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    void (*const begin)(uint16_t);
    void (*const flush)();
    uint8_t (*const read_byte)();
    size_t (*const read)(uint8_t*, size_t);
    size_t (*const write)(uint8_t*, size_t);
    void (*const end)();
} rgc_platform_uart_iface_t;

void rgc_platform_switch_gpio_setup();
void rgc_platform_switch_gpio_write(uint8_t state);

uint64_t rgc_platform_get_time_ms();

extern const rgc_platform_uart_iface_t rgc_platform_debug_uart_iface;
extern const rgc_platform_uart_iface_t rgc_platform_receiver_uart_iface;

#endif