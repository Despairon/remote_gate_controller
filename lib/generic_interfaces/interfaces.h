#ifndef GENERIC_INTERFACES_H
#define GENERIC_INTERFACES_H

#include <stdint.h>
#include <stddef.h>

typedef struct generic_switch_iface_s generic_switch_iface_t;
typedef struct generic_io_iface_s generic_io_iface_t;
typedef struct generic_time_iface_s generic_time_iface_t;
typedef struct generic_serial_iface_s generic_serial_iface_t;

typedef struct
{
    uint8_t *data;
    size_t size;
} buffer_t;

typedef union
{
    const generic_switch_iface_t *switch_iface;
    const generic_io_iface_t     *io_iface;
    const generic_time_iface_t   *time_iface;
    const generic_serial_iface_t *serial_iface;
} *generic_iface_handle_t;

typedef struct
{
    generic_iface_handle_t (*const create)(void*);
    void (*const destroy)(generic_iface_handle_t);
} generic_iface_ctor_dtor_t;

struct generic_switch_iface_s
{
    void    (*const turn_on)(generic_iface_handle_t);
    void    (*const turn_off)(generic_iface_handle_t);
    void    (*const toggle)(generic_iface_handle_t);
    uint8_t (*const get_state)(generic_iface_handle_t);
};

struct generic_io_iface_s
{
    size_t (*const send_data)(generic_iface_handle_t, buffer_t*);
    size_t (*const receive_data)(generic_iface_handle_t, buffer_t*);
    size_t (*const poll_available_data)(generic_iface_handle_t);
};

struct generic_time_iface_s
{
    uint64_t (*const get_time_ms)(generic_iface_handle_t);
};

struct generic_serial_iface_s
{
    void   (*const begin)(generic_iface_handle_t, uint32_t);
    size_t (*const available_for_read)(generic_iface_handle_t);
    size_t (*const write)(generic_iface_handle_t, buffer_t*);
    size_t (*const read)(generic_iface_handle_t, buffer_t*);
    void   (*const flush)(generic_iface_handle_t);
    void   (*const end)(generic_iface_handle_t);
};

#endif