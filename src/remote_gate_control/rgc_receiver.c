#include <remote_gate_control/rgc_receiver.h>
#include <remote_gate_control/rgc_common.h>
#include <remote_gate_control/rgc_platform_ifaces.h>
#include <esp8266/include/esp8266_driver.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

static esp8266_device_handle_t esp8266_driver = NULL;
static bool is_processing_trigger = false;
static uint64_t trigger_start_time = 0;

void rgc_receiver_init()
{
    is_processing_trigger = false;
    trigger_start_time = 0;

    rgc_platform_switch_gpio_setup();

    esp8266_device_init_data_t esp8266_init_data =
    {
        .platform_uart_begin     = &rgc_platform_receiver_uart_iface.begin,
        .platform_uart_flush     = &rgc_platform_receiver_uart_iface.flush,
        .platform_uart_read_byte = &rgc_platform_receiver_uart_iface.read_byte,
        .platform_uart_read      = &rgc_platform_receiver_uart_iface.read,
        .platform_uart_write     = &rgc_platform_receiver_uart_iface.write,
        .platform_uart_end       = &rgc_platform_receiver_uart_iface.end,
        .cmd_timeout             = RGC_RECEIVER_DEFAULT_TIMEOUT_MS
    };

    esp8266_driver = create_esp8266_device(&esp8266_init_data);

    if (esp8266_driver)
    {
        // TODO: set up esp8266 as Wi-Fi soft-AP, create a TCP server, listen to connections, and wait for the trigger message (RGC_TRIGGER)
    }
}

void rgc_receiver_tick()
{
    // TODO: implement

    esp8266_tick(esp8266_driver);

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