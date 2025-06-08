#ifndef ESP8266_DRIVER_H
#define ESP8266_DRIVER_H

#include "esp8266_common.h"
#include <interfaces.h>
#include <stdbool.h>

#define ESP8266_AT_SERIAL_IFACE_LINE_ENDING "\r\n"
#define ESP8266_AT_SERIAL_IFACE_MAX_READ_N 128U
#define ESP8266_AT_SERIAL_IFACE_BUFFER_SIZE (ESP8266_AT_SERIAL_IFACE_MAX_READ_N+1U)

struct esp8266_device_s;
typedef struct esp8266_device_s esp8266_device_t;
typedef esp8266_device_t* esp8266_device_handle_t;

typedef struct esp8266_device_init_data_s
{
    iface_ctx_t uart_ctx;
    iface_ctx_t time_iface_ctx;
    uint64_t cmd_timeout;
} esp8266_device_init_data_t;

typedef void (*esp8266_reset_callback_t)(esp8266_device_handle_t);
typedef void (*esp8266_get_wifi_mode_callback_t)(esp8266_device_handle_t, esp8266_wifi_mode_t mode);
typedef void (*esp8266_set_wifi_mode_callback_t)(esp8266_device_handle_t);

esp8266_device_handle_t create_esp8266_device(esp8266_device_init_data_t *init_data);

void esp8266_tick(esp8266_device_handle_t handle);
void esp8266_reset(esp8266_device_handle_t handle, esp8266_reset_callback_t cb);
void esp8266_get_wifi_mode(esp8266_device_handle_t handle, esp8266_get_wifi_mode_callback_t cb);
void esp8266_set_wifi_mode(esp8266_device_handle_t handle, esp8266_wifi_mode_t mode, esp8266_set_wifi_mode_callback_t cb);
bool esp8266_is_busy(esp8266_device_handle_t handle);

void destroy_esp8266_device(esp8266_device_handle_t handle);

#endif