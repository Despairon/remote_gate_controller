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

const fsm_transition_t esp8266_fsm_transitions[ESP8266_FSM_STATES_COUNT][ESP8266_FSM_EVENTS_COUNT] =
{
    [ESP8266_FSM_STATE_OFF] =
    {
        [ESP8266_FSM_EVENT_AWAKE]              = {ESP8266_FSM_STATE_OFF, &esp8266_fsm_at_ping},
        [ESP8266_FSM_EVENT_AT_OK]              = {ESP8266_FSM_STATE_READY, FSM_NO_EVENT_ACTION},
        [ESP8266_FSM_EVENT_AT_ERROR]           = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_READY]           = {ESP8266_FSM_STATE_READY, FSM_NO_EVENT_ACTION}, // TODO: process reset completion here, call reset cb
        [ESP8266_FSM_EVENT_AT_CWMODE_RECEIVED] = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_CMD_RESET]          = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_CMD_GET_WIFI_MODE]  = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_CMD_SET_WIFI_MODE]  = FSM_NO_TRANSITION
    },

    [ESP8266_FSM_STATE_READY] =
    {
        [ESP8266_FSM_EVENT_AWAKE]              = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_OK]              = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_ERROR]           = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_READY]           = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_CWMODE_RECEIVED] = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_CMD_RESET]          = {ESP8266_FSM_STATE_RESET, &esp8266_fsm_at_reset},
        [ESP8266_FSM_EVENT_CMD_GET_WIFI_MODE]  = {ESP8266_FSM_STATE_CMD_GET_WIFI_MODE, &esp8266_fsm_at_get_cwmode},
        [ESP8266_FSM_EVENT_CMD_SET_WIFI_MODE]  = {ESP8266_FSM_STATE_CMD_SET_WIFI_MODE, &esp8266_fsm_at_set_cwmode}
    },
    
    [ESP8266_FSM_STATE_CMD_GET_WIFI_MODE] =
    {
        [ESP8266_FSM_EVENT_AWAKE]              = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_OK]              = {ESP8266_FSM_STATE_READY, FSM_NO_EVENT_ACTION},
        [ESP8266_FSM_EVENT_AT_ERROR]           = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_READY]           = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_CWMODE_RECEIVED] = {ESP8266_FSM_STATE_CMD_GET_WIFI_MODE, &esp8266_fsm_at_get_cwmode_complete},
        [ESP8266_FSM_EVENT_CMD_RESET]          = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_CMD_GET_WIFI_MODE]  = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_CMD_SET_WIFI_MODE]  = FSM_NO_TRANSITION        
    },
    
    [ESP8266_FSM_STATE_CMD_SET_WIFI_MODE] =
    {
        [ESP8266_FSM_EVENT_AWAKE]              = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_OK]              = {ESP8266_FSM_STATE_READY, &esp8266_fsm_at_set_cwmode_complete},
        [ESP8266_FSM_EVENT_AT_ERROR]           = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_READY]           = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_CWMODE_RECEIVED] = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_CMD_RESET]          = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_CMD_GET_WIFI_MODE]  = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_CMD_SET_WIFI_MODE]  = FSM_NO_TRANSITION        
    },

    [ESP8266_FSM_STATE_RESET] =
    {
        [ESP8266_FSM_EVENT_AWAKE]              = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_OK]              = {ESP8266_FSM_STATE_OFF, FSM_NO_EVENT_ACTION},
        [ESP8266_FSM_EVENT_AT_ERROR]           = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_READY]           = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_AT_CWMODE_RECEIVED] = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_CMD_RESET]          = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_CMD_GET_WIFI_MODE]  = FSM_NO_TRANSITION,
        [ESP8266_FSM_EVENT_CMD_SET_WIFI_MODE]  = FSM_NO_TRANSITION
    }
};

static inline bool serial_send_str(const char *str, generic_iface_handle_t serial_iface_handle)
{
    size_t str_size = strlen(str);
    buffer_t buffer = {(uint8_t*)str, str_size};

    return str_size == serial_iface_handle->serial_iface->write(serial_iface_handle, &buffer);  
}

static inline bool send_at_cmd_execute(const char *cmd, generic_iface_handle_t serial_iface_handle)
{
    char cmd_out[AT_CMD_SIZE] = {'\0'};

    (void)strcat(at_command_execute(cmd, cmd_out), ESP8266_AT_SERIAL_IFACE_LINE_ENDING);

    return serial_send_str(cmd_out, serial_iface_handle);
}

static inline bool send_at_cmd_inquiry(const char *cmd, generic_iface_handle_t serial_iface_handle)
{
    char cmd_out[AT_CMD_SIZE] = {'\0'};

    (void)strcat(at_command_inquiry(cmd, cmd_out), ESP8266_AT_SERIAL_IFACE_LINE_ENDING);

    return serial_send_str(cmd_out, serial_iface_handle); 
}

static inline bool send_at_cmd_set(const char *cmd, char *data, generic_iface_handle_t serial_iface_handle)
{
    char cmd_out[AT_CMD_SIZE] = {'\0'};

    (void)strcat(at_command_set(cmd, data, cmd_out), ESP8266_AT_SERIAL_IFACE_LINE_ENDING);

    return serial_send_str(cmd_out, serial_iface_handle); 
}

static void esp8266_fsm_at_ping(void *event_data)
{
    if (!event_data)
        return;

    generic_iface_handle_t serial_iface_handle = ((esp8266_fsm_event_data_t*)event_data)->base.dev_init_data->serial_iface_handle;

    if (!serial_send_str(AT_CMD ESP8266_AT_SERIAL_IFACE_LINE_ENDING, serial_iface_handle))
    {
        // TODO: error handling
    }
}

static void esp8266_fsm_at_reset(void *event_data)
{
    if (!event_data)
        return;
    
    generic_iface_handle_t serial_iface_handle = ((esp8266_fsm_event_data_t*)event_data)->base.dev_init_data->serial_iface_handle;

    if (!send_at_cmd_execute(ESP8266_AT_CMD_RESET, serial_iface_handle))
    {
        // TODO: error handling, callback usage
    }
}

static void esp8266_fsm_at_get_cwmode(void *event_data)
{
    if (!event_data)
        return;
    
    generic_iface_handle_t serial_iface_handle = ((esp8266_fsm_event_data_t*)event_data)->base.dev_init_data->serial_iface_handle;

    if (!send_at_cmd_inquiry(ESP8266_AT_CMD_CWMODE, serial_iface_handle))
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
    
    generic_iface_handle_t serial_iface_handle = ((esp8266_fsm_event_data_t*)event_data)->base.dev_init_data->serial_iface_handle;

    uint8_t cwmode_val = ((esp8266_fsm_event_data_t*)event_data)->data_variant.byte;
    
    char cmd_data[] = {'0'+cwmode_val, '\0'};

    if (!send_at_cmd_set(ESP8266_AT_CMD_CWMODE, cmd_data, serial_iface_handle))
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