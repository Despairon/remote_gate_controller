#include <remote_gate_control/rgc_receiver.h>
#include <remote_gate_control/rgc_common.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct rgc_receiver_data_s
{
    rgc_receiver_init_data_t init_data;
    bool is_processing_trigger;
    uint64_t trigger_start_time;
};

rgc_receiver_handle_t rgc_receiver_init(rgc_receiver_init_data_t *init_data)
{
    rgc_receiver_handle_t rgc_receiver = NULL;

    if (init_data)
    {
        if (init_data->switch_handle && init_data->generic_io_handle && init_data->time_iface_handle)
        {
            if (init_data->switch_handle->switch_iface && init_data->generic_io_handle->io_iface && init_data->time_iface_handle->time_iface)
            {
                rgc_receiver = (rgc_receiver_data_t*)malloc(sizeof(rgc_receiver_data_t));
                
                if (rgc_receiver)
                {
                    rgc_receiver->init_data = *init_data;

                    rgc_receiver->init_data.trigger_delay = init_data->trigger_delay < RGC_TRIGGER_DELAY_MIN ? RGC_TRIGGER_DELAY_MIN 
                                                          : init_data->trigger_delay > RGC_TRIGGER_DELAY_MAX ? RGC_TRIGGER_DELAY_MAX
                                                          : init_data->trigger_delay;
            
                    rgc_receiver->init_data.switch_handle->switch_iface->turn_off(rgc_receiver->init_data.switch_handle);
                }
            }
        }
    }

    return rgc_receiver;
}

void rgc_receiver_deinit(rgc_receiver_handle_t rgc_receiver)
{
    if (rgc_receiver)
        free((rgc_receiver_data_t*)rgc_receiver);
}

void rgc_receiver_tick(rgc_receiver_handle_t rgc_receiver)
{
    if (!rgc_receiver)
        return;

    generic_iface_handle_t switch_iface_handle = rgc_receiver->init_data.switch_handle;
    generic_iface_handle_t generic_io_iface_handle = rgc_receiver->init_data.generic_io_handle;
    generic_iface_handle_t time_iface_handle = rgc_receiver->init_data.time_iface_handle;

    const generic_switch_iface_t *switch_iface = switch_iface_handle->switch_iface;
    const generic_io_iface_t *generic_io_iface = generic_io_iface_handle->io_iface;
    const generic_time_iface_t *time_iface = time_iface_handle->time_iface;

    size_t available_data_sz = generic_io_iface->poll_available_data(generic_io_iface_handle); // TODO: think how to maybe change that, but it's needed for running internal "tick" function of the underlying io implementation..

    if (!(rgc_receiver->is_processing_trigger) && (available_data_sz >= RGC_TRIGGER_CMD_SIZE))
    {
        uint8_t data_buf[RGC_TRIGGER_CMD_SIZE] = {0};
        buffer_t buffer = {data_buf, sizeof(data_buf)};

        size_t sz_received = generic_io_iface->receive_data(generic_io_iface_handle, &buffer);
        if (sz_received == sizeof(data_buf))
        {
            if (memcmp((const void*)data_buf, (const void*)RGC_TRIGGER_CMD, RGC_TRIGGER_CMD_SIZE) != 0)
            {
                switch_iface->turn_on(switch_iface_handle);
                rgc_receiver->is_processing_trigger = true;
                rgc_receiver->trigger_start_time = time_iface->get_time_ms(time_iface_handle);
            }
        }
    }
    else
    {
        if ((time_iface->get_time_ms(time_iface_handle) - rgc_receiver->trigger_start_time) > rgc_receiver->init_data.trigger_delay)
        {
            switch_iface->turn_off(switch_iface_handle);
            rgc_receiver->is_processing_trigger = false;
            rgc_receiver->trigger_start_time = 0;
        }
    }
}
