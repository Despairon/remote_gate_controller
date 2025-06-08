#ifndef ESP8266_DRIVER_FSM_h
#define ESP8266_DRIVER_FSM_H

#include <simple_fsm_v1.h>
#include <esp8266/include/esp8266_driver.h>

typedef enum esp8266_fsm_state_e
{
    ESP8266_FSM_STATE_OFF,
    ESP8266_FSM_STATE_READY,
    ESP8266_FSM_STATE_RESET,
    ESP8266_FSM_STATE_CMD_GET_WIFI_MODE,
    ESP8266_FSM_STATE_CMD_SET_WIFI_MODE
} esp8266_fsm_state_t;

typedef enum esp8266_fsm_event_e
{
    ESP8266_FSM_EVENT_AWAKE,
    ESP8266_FSM_EVENT_AT_OK,
    ESP8266_FSM_EVENT_AT_ERROR,
    ESP8266_FSM_EVENT_AT_READY,
    ESP8266_FSM_EVENT_AT_CWMODE_RECEIVED,
    ESP8266_FSM_EVENT_CMD_RESET,
    ESP8266_FSM_EVENT_CMD_GET_WIFI_MODE,
    ESP8266_FSM_EVENT_CMD_SET_WIFI_MODE,

    ESP8266_FSM_NO_EVENT = FSM_NULL_EVENT
} esp8266_fsm_event_t;

typedef struct esp8266_fsm_event_data_base_s
{
    esp8266_device_init_data_t *dev_init_data;
} esp8266_fsm_event_data_base_t;

typedef struct esp8266_fsm_event_data_s
{
    esp8266_fsm_event_data_base_t base;
    union
    {
        buffer_t buffer;
        char *str;
        uint8_t byte;
    } data_variant;
} esp8266_fsm_event_data_t;

extern const fsm_transition_t esp8266_fsm_transitions[];
extern const size_t esp8266_fsm_transitions_size;

#endif
