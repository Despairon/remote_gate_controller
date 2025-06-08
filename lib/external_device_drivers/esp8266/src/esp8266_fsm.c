#include <esp8266/include/esp8266_fsm.h>
#include <at_commands.h>

#define ESP8266_AT_CMD_RESET "RST"
#define ESP8266_AT_CMD_CWMODE "CWMODE"

static void esp8266_fsm_at_ping(void *event_data);
static void esp8266_fsm_at_reset(void *event_data);
static void esp8266_fsm_at_get_cwmode(void *event_data);
static void esp8266_fsm_at_get_cwmode_complete(void *event_data);
static void esp8266_fsm_at_set_cwmode(void *event_data);
static void esp8266_fsm_at_set_cwmode_complete(void *event_data);

const fsm_transition_t esp8266_fsm_transitions[] =
{
    {ESP8266_FSM_STATE_OFF, ESP8266_FSM_EVENT_AWAKE,    ESP8266_FSM_STATE_OFF,   &esp8266_fsm_at_ping},
    {ESP8266_FSM_STATE_OFF, ESP8266_FSM_EVENT_AT_OK,    ESP8266_FSM_STATE_READY, FSM_NO_EVENT_ACTION},
    {ESP8266_FSM_STATE_OFF, ESP8266_FSM_EVENT_AT_READY, ESP8266_FSM_STATE_READY, FSM_NO_EVENT_ACTION}, // TODO: process reset completion here, call reset cb

    {ESP8266_FSM_STATE_READY, ESP8266_FSM_EVENT_CMD_RESET,         ESP8266_FSM_STATE_RESET,             &esp8266_fsm_at_reset},
    {ESP8266_FSM_STATE_READY, ESP8266_FSM_EVENT_CMD_GET_WIFI_MODE, ESP8266_FSM_STATE_CMD_GET_WIFI_MODE, &esp8266_fsm_at_get_cwmode},
    {ESP8266_FSM_STATE_READY, ESP8266_FSM_EVENT_CMD_SET_WIFI_MODE, ESP8266_FSM_STATE_CMD_SET_WIFI_MODE, &esp8266_fsm_at_set_cwmode},

    {ESP8266_FSM_STATE_CMD_GET_WIFI_MODE, ESP8266_FSM_EVENT_AT_OK,              ESP8266_FSM_STATE_READY,             FSM_NO_EVENT_ACTION},
    {ESP8266_FSM_STATE_CMD_GET_WIFI_MODE, ESP8266_FSM_EVENT_AT_CWMODE_RECEIVED, ESP8266_FSM_STATE_CMD_GET_WIFI_MODE, &esp8266_fsm_at_get_cwmode_complete},
    
    {ESP8266_FSM_STATE_CMD_SET_WIFI_MODE, ESP8266_FSM_EVENT_AT_OK, ESP8266_FSM_STATE_READY, &esp8266_fsm_at_set_cwmode_complete},

    {ESP8266_FSM_STATE_RESET, ESP8266_FSM_EVENT_AT_OK, ESP8266_FSM_STATE_OFF, FSM_NO_EVENT_ACTION}
};
const size_t esp8266_fsm_transitions_size = sizeof(esp8266_fsm_transitions) / sizeof(esp8266_fsm_transitions[0]);

static inline bool uart_send_str(const char *str, iface_ctx_t uart_ctx)
{
    size_t str_size = strlen(str);
    buffer_t buffer = {(uint8_t*)str, str_size};

    return str_size == uart_ctx->uart_iface->write(uart_ctx, &buffer);  
}

static inline bool send_at_cmd_execute(const char *cmd, iface_ctx_t uart_ctx)
{
    char cmd_out[AT_CMD_SIZE] = {'\0'};

    (void)strcat(at_command_execute(cmd, cmd_out), ESP8266_AT_SERIAL_IFACE_LINE_ENDING);

    return uart_send_str(cmd_out, uart_ctx);
}

static inline bool send_at_cmd_inquiry(const char *cmd, iface_ctx_t uart_ctx)
{
    char cmd_out[AT_CMD_SIZE] = {'\0'};

    (void)strcat(at_command_inquiry(cmd, cmd_out), ESP8266_AT_SERIAL_IFACE_LINE_ENDING);

    return uart_send_str(cmd_out, uart_ctx); 
}

static inline bool send_at_cmd_set(const char *cmd, char *data, iface_ctx_t uart_ctx)
{
    char cmd_out[AT_CMD_SIZE] = {'\0'};

    (void)strcat(at_command_set(cmd, data, cmd_out), ESP8266_AT_SERIAL_IFACE_LINE_ENDING);

    return uart_send_str(cmd_out, uart_ctx); 
}

static void esp8266_fsm_at_ping(void *event_data)
{
    if (!event_data)
        return;

    iface_ctx_t uart_ctx = ((esp8266_fsm_event_data_t*)event_data)->base.dev_init_data->uart_ctx;

    if (!uart_send_str(AT_CMD ESP8266_AT_SERIAL_IFACE_LINE_ENDING, uart_ctx))
    {
        // TODO: error handling
    }
}

static void esp8266_fsm_at_reset(void *event_data)
{
    if (!event_data)
        return;
    
    iface_ctx_t uart_ctx = ((esp8266_fsm_event_data_t*)event_data)->base.dev_init_data->uart_ctx;

    if (!send_at_cmd_execute(ESP8266_AT_CMD_RESET, uart_ctx))
    {
        // TODO: error handling, callback usage
    }
}

static void esp8266_fsm_at_get_cwmode(void *event_data)
{
    if (!event_data)
        return;
    
    iface_ctx_t uart_ctx = ((esp8266_fsm_event_data_t*)event_data)->base.dev_init_data->uart_ctx;

    if (!send_at_cmd_inquiry(ESP8266_AT_CMD_CWMODE, uart_ctx))
    {
        // TODO: error handling, callback usage
    }
}

static void esp8266_fsm_at_get_cwmode_complete(void *event_data)
{
    if (!event_data)
        return;
    // TODO: finish implementation: error handling, callback usage
}

static void esp8266_fsm_at_set_cwmode(void *event_data)
{
    if (!event_data)
        return;
    
    iface_ctx_t uart_ctx = ((esp8266_fsm_event_data_t*)event_data)->base.dev_init_data->uart_ctx;

    uint8_t cwmode_val = ((esp8266_fsm_event_data_t*)event_data)->data_variant.byte;
    
    char cmd_data[] = {'0'+cwmode_val, '\0'};

    if (!send_at_cmd_set(ESP8266_AT_CMD_CWMODE, cmd_data, uart_ctx))
    {
        // TODO: error handling, callback usage
    }
}

static void esp8266_fsm_at_set_cwmode_complete(void *event_data)
{
    if (!event_data)
        return;
    // TODO: finish implementation: error handling, callback usage
}