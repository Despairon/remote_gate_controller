#include <remote_gate_control/generic_switch_iface_impl.h>
#include <stdlib.h>
#include <Arduino.h>

typedef struct generic_switch_data_s
{
    const generic_switch_iface_t *iface;
    generic_switch_iface_init_data_t init_data;
    int state;
} generic_switch_data_t;

static generic_iface_handle_t create_generic_switch(void *init_data);
static void destroy_generic_switch(generic_iface_handle_t handle);

static void generic_switch_turn_on(generic_iface_handle_t handle);
static void generic_switch_turn_off(generic_iface_handle_t handle);
static void generic_switch_toggle(generic_iface_handle_t handle);
static uint8_t generic_switch_get_state(generic_iface_handle_t handle);

static void generic_switch_set_state(generic_iface_handle_t handle, int state);

const generic_iface_ctor_dtor_t generic_switch_iface_ctor_dtor =
{
    .create  = &create_generic_switch,
    .destroy = &destroy_generic_switch
};

static const generic_switch_iface_t generic_switch_iface =
{
    .turn_on   = &generic_switch_turn_on,
    .turn_off  = &generic_switch_turn_off,
    .toggle    = &generic_switch_toggle,
    .get_state = &generic_switch_get_state
};

static generic_iface_handle_t create_generic_switch(void *init_data)
{
    generic_iface_handle_t handle = NULL;

    if (init_data)
    {
        generic_switch_data_t *iface_data = (generic_switch_data_t*)malloc(sizeof(generic_switch_data_t));
        if (iface_data)
        {
            iface_data->iface = &generic_switch_iface;

            (void)memcpy(&(iface_data->init_data), init_data, sizeof(generic_switch_iface_init_data_t));

            pinMode(iface_data->init_data.pin_num, OUTPUT);
            
            handle = (generic_iface_handle_t)iface_data;
            
            generic_switch_set_state(handle, LOW);
        }
    }

    return handle;
}

static void destroy_generic_switch(generic_iface_handle_t handle)
{
    if (handle)
        free((generic_switch_data_t*)handle);
}

static void generic_switch_turn_on(generic_iface_handle_t handle)
{
    generic_switch_set_state(handle, HIGH);
}

static void generic_switch_turn_off(generic_iface_handle_t handle)
{
    generic_switch_set_state(handle, LOW);
}

static void generic_switch_toggle(generic_iface_handle_t handle)
{
    if (handle)
        ((generic_switch_data_t*)handle)->state ? generic_switch_turn_off(handle) : generic_switch_turn_on(handle);
}

static uint8_t generic_switch_get_state(generic_iface_handle_t handle)
{
    uint8_t state = -1;

    if (handle)
        state = ((generic_switch_data_t*)handle)->state;
    
    return state;
}

static void generic_switch_set_state(generic_iface_handle_t handle, int state)
{
    if (handle)
    {
        generic_switch_data_t *switch_data = (generic_switch_data_t*)handle;

        if (switch_data->state != state)
        {
            digitalWrite(switch_data->init_data.pin_num, state ? HIGH : LOW);
            switch_data->state = state ? HIGH : LOW;
        }
    }
}