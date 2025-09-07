#include <rgc_transmitter.h>
#include <rgc_common.h>
#include <rgc_platform_ifaces.h>
#include <stdio.h>
#include <string.h>

static bool new_button_state = false;
static bool is_button_pressed = false;
static uint64_t button_debounce_start_time = 0;

void rgc_transmitter_init()
{
    rgc_platform_transmitter_button_setup();
    rgc_platform_debug_uart_iface.begin(RGC_PLATFORM_DEBUG_UART_BAUDRATE);
    rgc_platform_transmitter_uart_iface.begin(RGC_PLATFORM_TRANSMITTER_UART_BAUDRATE);
}

void rgc_transmitter_tick()
{
    new_button_state = rgc_platform_transmitter_is_button_pressed();

    if (!button_debounce_start_time) // TODO: Check button connection to the board - it is loose
    {
        if (new_button_state != is_button_pressed)
            button_debounce_start_time = rgc_platform_get_time_ms();
    }
    else
    {
        if (((rgc_platform_get_time_ms() - button_debounce_start_time) >= RGC_TRANSMITTER_BUTTON_DEBOUNCE_MS))
        {
            if (new_button_state != is_button_pressed)
            {
                is_button_pressed = new_button_state;

                // TODO: add on button press reaction

                // TODO: Debug prints, can be removed after the button is no longer connected loosely.
                uint8_t msg[64] = {'\0'};
                snprintf((char*)msg, sizeof(msg), "Button state changed, it is now %s\r\n", is_button_pressed ? "pressed" : "released");
                rgc_platform_debug_uart_iface.write(msg, strlen(msg));
            }

            button_debounce_start_time = 0;
        }
    }

    // Debug prints just in case
    // static uint64_t cur_ms = 0;
    // if (cur_ms != rgc_platform_get_time_ms())
    // {
    //     cur_ms = rgc_platform_get_time_ms();
    //     if ((cur_ms % 500) == 0)
    //     {
    //         uint8_t msg[32] = {'\0'};
    //         snprintf((char*)msg, sizeof(msg), "Button is %s\r\n", is_button_pressed ? "pressed" : "released");
    //         rgc_platform_debug_uart_iface.write(msg, strlen(msg));
    //     }
    // }
}