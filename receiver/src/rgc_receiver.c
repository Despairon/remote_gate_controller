#include <rgc_receiver.h>
#include <rgc_common.h>
#include <rgc_platform_ifaces.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

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
    if (rgc_platform_receiver_uart_iface.data_available())
    {
        uint8_t data_buf[RGC_TRIGGER_CMD_SIZE] = {0};

        size_t sz_read = rgc_platform_receiver_uart_iface.read(data_buf, sizeof(data_buf));

        if (sz_read)
        {
            uint8_t msg[64] = {'\0'};
            (void)snprintf((char*)msg, sizeof(msg), "Received %d bytes: %s\r\n", sz_read, data_buf);
            rgc_platform_debug_uart_iface.write(msg, strlen((char*)msg));
        }

        if ((sz_read == sizeof(data_buf))
        && (memcmp((const void*)data_buf, (const void*)RGC_TRIGGER_CMD, sizeof(data_buf)) == 0)
        &&  !is_processing_trigger)
        {
            rgc_platform_switch_gpio_write(0);
            is_processing_trigger = true;
            trigger_start_time = rgc_platform_get_time_ms();
        }
    }

    if (is_processing_trigger)
    {
        if ((rgc_platform_get_time_ms() - trigger_start_time) >= RGC_TRIGGER_DELAY_MS)
        {
            rgc_platform_switch_gpio_write(1);
            is_processing_trigger = false;
            trigger_start_time = 0;
        }
    }
}