#ifndef GENERIC_INTERFACES_H
#define GENERIC_INTERFACES_H

#include <stdint.h>
#include <stddef.h>

struct generic_io_s;
struct time_iface_s;
struct gpio_iface_s;
struct uart_iface_s;
struct interface_s;

typedef struct generic_io_s generic_io_t;
typedef struct time_iface_s time_iface_t;
typedef struct gpio_iface_s gpio_iface_t;
typedef struct uart_iface_s uart_iface_t;
typedef struct interface_s interface_t;
typedef void* iface_ctx_t; // iface context

typedef struct
{
    uint8_t *data;
    size_t size;
} buffer_t;

typedef struct
{
    iface_ctx_t (*const create)(void*);
    void (*const destroy)(iface_ctx_t);
} init_iface_t;

struct generic_io_s
{
    size_t (*const read)(iface_ctx_t, void*);
    size_t (*const write)(iface_ctx_t, void*);
};

struct time_iface_s
{
    uint64_t (*const get_time_ms)(iface_ctx_t);
};

struct gpio_iface_s
{
    generic_io_t blocking_io;

    void (*const toggle)(iface_ctx_t);
};

struct uart_iface_s
{
    generic_io_t blocking_io;

    void (*const abort)(iface_ctx_t);
    void (*const abort_rx)(iface_ctx_t);
    void (*const abort_tx)(iface_ctx_t);
};

struct interface_s
{
    const init_iface_t init;

    union
    {
        const generic_io_t io_iface;
        const time_iface_t time_iface;
        const gpio_iface_t gpio_iface;
        const uart_iface_t uart_iface;
    } const variant;
};

#endif