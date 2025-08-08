#include <esp8266/include/esp8266_driver.h>
#include <esp8266/include/esp8266_fsm.h>
#include <stdlib.h>
#include <string.h>

#define ESP8266_AT_SERIAL_IFACE_SPEED 115200U
#define ESP8266_AT_CMD_DELIM ':'

struct esp8266_device_s
{
    esp8266_device_init_data_t init_data;
    state_machine_t state_machine;
    uint8_t uart_buffer[ESP8266_AT_SERIAL_IFACE_BUFFER_SIZE];
};

static void process_uart_input_line(esp8266_device_handle_t handle, char *str, esp8266_fsm_event_t *event_id_out, esp8266_fsm_event_data_t *event_data_out);

static inline esp8266_fsm_event_data_t get_fsm_event_data_base(esp8266_device_handle_t handle)
{
    esp8266_fsm_event_data_t event_data =
    {
        .base = {.dev_init_data = &(handle->init_data)},
        .data_variant = {{0}}
    };

    return event_data;
}

static inline bool is_init_data_valid(esp8266_device_init_data_t *init_data)
{
    bool res = init_data != NULL;

    if (res)
    {
        if (init_data->platform_uart_begin
        &&  init_data->platform_uart_flush
        &&  init_data->platform_uart_read_byte
        &&  init_data->platform_uart_read
        &&  init_data->platform_uart_write
        &&  init_data->platform_uart_end
        &&  init_data->platform_get_time_ms
        &&  init_data->cmd_timeout)
        {
            res = true;
        }
    }

    return res;
}

esp8266_device_handle_t create_esp8266_device(esp8266_device_init_data_t *init_data)
{
    struct esp8266_device_s *handle = NULL;

    if (is_init_data_valid(init_data))
    {
        handle = (esp8266_device_handle_t)malloc(sizeof(esp8266_device_t));
        if (handle)
        {
            (void)memcpy(&(handle->init_data), init_data, sizeof(esp8266_device_init_data_t));

            handle->state_machine = (const state_machine_t){ESP8266_FSM_STATE_OFF, (const fsm_transition_t*)esp8266_fsm_transitions, esp8266_fsm_transitions_size};

            (void)memset(handle->uart_buffer, '\0', sizeof(handle->uart_buffer));

            handle->init_data.platform_uart_begin(ESP8266_AT_SERIAL_IFACE_SPEED);
            handle->init_data.platform_uart_flush();

            esp8266_fsm_event_data_t event_data = get_fsm_event_data_base(handle);
            fsm_process_event(&(handle->state_machine), (uint16_t)ESP8266_FSM_EVENT_AWAKE, (void*)&event_data);
        }
    }

    return handle;
}

void esp8266_tick(esp8266_device_handle_t handle)
{
    if (!handle)
        return;
    // TODO: need to rework a little...
    iface_ctx_t uart_ctx = handle->init_data.uart_ctx;
    
    const uart_iface_t *uart = uart_ctx->uart_iface;
    
    uint8_t *uart_buffer_end = &(handle->uart_buffer[sizeof(handle->uart_buffer)-1]);

    size_t available_bytes_for_read = uart->available_for_read(uart_ctx);
    if (available_bytes_for_read)
    {
        uint8_t *free_buffer_space_start = (uint8_t*)strchr((const char*)(handle->uart_buffer), '\0');

        size_t free_buffer_bytes = (size_t)uart_buffer_end - (size_t)free_buffer_space_start - 1;
        if (free_buffer_bytes)
        {
            buffer_t read_buf = {free_buffer_space_start, free_buffer_bytes > available_bytes_for_read ? available_bytes_for_read : free_buffer_bytes};
            (void)uart->read(uart_ctx, &read_buf);
        }
        else
        {
            // TODO: error handling, buffer is full
        }
    }

    size_t filled_buffer_bytes = (size_t)(strchr((const char*)(handle->uart_buffer), '\0') - (char*)(handle->uart_buffer));
    if (filled_buffer_bytes)
    {
        char *line_end = strstr((const char*)(handle->uart_buffer), ESP8266_AT_SERIAL_IFACE_LINE_ENDING);
        if (line_end)
        {
            *line_end = '\0';
            *(++line_end) = '\0';

            esp8266_fsm_event_t event = ESP8266_FSM_NO_EVENT;
            esp8266_fsm_event_data_t event_data = get_fsm_event_data_base(handle);

            process_uart_input_line(handle, (char*)(handle->uart_buffer), &event, &event_data);

            if (event != ESP8266_FSM_NO_EVENT)
                fsm_process_event(&(handle->state_machine), (uint16_t)event, (void*)&event_data);

            uint8_t *buf_byte = handle->uart_buffer;
            while (++line_end < (char*)uart_buffer_end)
            {
                if (*line_end == '\0')
                    break;
                
                *buf_byte = (uint8_t)*line_end;
                
                ++buf_byte;

                *line_end = '\0';
            }
        }
    }
}

void esp8266_reset(esp8266_device_handle_t handle, esp8266_reset_callback_t cb)
{
    if (!handle)
        return;
    
    if (esp8266_is_busy(handle))
        return;

    esp8266_fsm_event_data_t event_data = get_fsm_event_data_base(handle);
    fsm_process_event(&(handle->state_machine), (uint16_t)ESP8266_FSM_EVENT_CMD_RESET, (void*)&event_data);
    // TODO: finish implementation, add callback, error handling
}

void esp8266_get_wifi_mode(esp8266_device_handle_t handle, esp8266_get_wifi_mode_callback_t cb)
{
    if (!handle)
        return;

    if (esp8266_is_busy(handle))
        return;
    
    esp8266_fsm_event_data_t event_data = get_fsm_event_data_base(handle);
    fsm_process_event(&(handle->state_machine), (uint16_t)ESP8266_FSM_EVENT_CMD_GET_WIFI_MODE, (void*)&event_data);
    // TODO: finish implementation, add callback, error handling
}

void esp8266_set_wifi_mode(esp8266_device_handle_t handle, esp8266_wifi_mode_t mode, esp8266_set_wifi_mode_callback_t cb)
{
    if (!handle)
        return;

    if (esp8266_is_busy(handle))
        return;

    esp8266_fsm_event_data_t event_data = get_fsm_event_data_base(handle);

    event_data.data_variant.byte = (uint8_t)mode;

    fsm_process_event(&(handle->state_machine), (uint16_t)ESP8266_FSM_EVENT_CMD_SET_WIFI_MODE, (void*)&event_data);
    // TODO: finish implementation, e.g. add callback, error handling
}

bool esp8266_is_busy(esp8266_device_handle_t handle)
{
    bool is_busy = true;
    
    if (handle)
        is_busy = handle->state_machine.current_state != ESP8266_FSM_STATE_READY;

    return is_busy;
}

void destroy_esp8266_device(esp8266_device_handle_t handle)
{
    if (!handle)
        return;

    iface_ctx_t uart_ctx = handle->init_data.uart_ctx;

    uart_ctx->uart_iface->end(uart_ctx);

    free((esp8266_device_t*)handle);
}

static void process_uart_input_line(esp8266_device_handle_t handle, char *str, esp8266_fsm_event_t *event_id_out, esp8266_fsm_event_data_t *event_data_out)
{
    struct
    {
        const char *input_str;
        esp8266_fsm_event_t event;
    } static const input_to_event_map[] =
    {
        {"OK",      ESP8266_FSM_EVENT_AT_OK},
        {"ready",   ESP8266_FSM_EVENT_AT_READY},
        {"ERROR",   ESP8266_FSM_EVENT_AT_ERROR},
        {"+CWMODE", ESP8266_FSM_EVENT_AT_CWMODE_RECEIVED}
    };
    static const size_t input_to_event_map_len = sizeof(input_to_event_map) / sizeof(input_to_event_map[0]);

    if (handle && str && event_id_out && event_data_out)
    {
        size_t i = 0;
        for (; i < input_to_event_map_len; i++)
        {
            char *match_str = strstr(str, input_to_event_map[i].input_str);
            if (match_str && (match_str == str))
            {
                *event_id_out = input_to_event_map[i].event;
                *event_data_out = get_fsm_event_data_base(handle);

                char *input_data_begin = strchr(str, ESP8266_AT_CMD_DELIM);
                if (input_data_begin++)
                {
                    switch (*event_id_out)
                    {
                        case ESP8266_FSM_EVENT_AT_CWMODE_RECEIVED:
                        {
                            event_data_out->data_variant.byte = (uint8_t)((int)*input_data_begin - '0');
                        }
                        break;
    
                        default:
                        break;
                    }
                }
            }
        }
    }
}