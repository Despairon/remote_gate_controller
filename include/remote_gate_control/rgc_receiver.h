#ifndef RGC_RECEIVER_H
#define RGC_RECEIVER_H

#include <interfaces.h>
#include <stdbool.h>

struct rgc_receiver_data_s;
typedef struct rgc_receiver_data_s rgc_receiver_data_t;

typedef struct rgc_receiver_init_data_s
{
    iface_ctx_t gpio_ctx;
    iface_ctx_t receiver_io_ctx;
    iface_ctx_t time_iface_ctx;
    uint64_t trigger_delay;
} rgc_receiver_init_data_t;

typedef rgc_receiver_data_t* rgc_receiver_handle_t;

rgc_receiver_handle_t rgc_receiver_init(rgc_receiver_init_data_t *init_data);
void rgc_receiver_deinit(rgc_receiver_handle_t rgc_receiver);

void rgc_receiver_tick(rgc_receiver_handle_t rgc_receiver);

#endif