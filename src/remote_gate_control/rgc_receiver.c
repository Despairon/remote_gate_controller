#include <remote_gate_control/rgc_receiver.h>
#include <remote_gate_control/rgc_common.h>
#include <remote_gate_control/rgc_platform_ifaces.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

static bool is_processing_trigger = false;
static uint64_t trigger_start_time = 0;

void rgc_receiver_init()
{
    is_processing_trigger = false;
    trigger_start_time = 0;

    rgc_platform_switch_gpio_setup();

    rgc_platform_debug_uart_iface.begin(RGC_PLATFORM_DEBUG_UART_BAUDRATE);
    rgc_platform_receiver_uart_iface.begin(RGC_PLATFORM_RECEIVER_UART_BAUDRATE);
}

void rgc_receiver_tick()
{
    // TODO: implement

    // if (!is_processing_trigger)
    // {
    //     uint8_t data_buf[RGC_TRIGGER_CMD_SIZE] = {0};
    //     buffer_t buffer = {data_buf, sizeof(data_buf)};

    //     size_t sz_received = generic_io->receive_data(receiver_io_ctx, &buffer);
    //     if (sz_received == sizeof(data_buf))
    //     {
    //         if (memcmp((const void*)data_buf, (const void*)RGC_TRIGGER_CMD, RGC_TRIGGER_CMD_SIZE) != 0)
    //         {
    //             gpio_iface->turn_on(gpio_iface_ctx);
    //             rgc_receiver->is_processing_trigger = true;
    //             rgc_receiver->trigger_start_time = time_iface->get_time_ms(time_iface_ctx);
    //         }
    //     }
    // }
    // else
    // {
    //     if ((time_iface->get_time_ms(time_iface_ctx) - rgc_receiver->trigger_start_time) > rgc_receiver->init_data.trigger_delay)
    //     {
    //         gpio_iface->turn_off(gpio_iface_ctx);
    //         rgc_receiver->is_processing_trigger = false;
    //         rgc_receiver->trigger_start_time = 0;
    //     }
    // }
}