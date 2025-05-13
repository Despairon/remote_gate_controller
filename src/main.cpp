#include <Arduino.h>
extern "C"
{    
    #include <remote_gate_control/rgc_receiver.h>
}
#include <remote_gate_control/generic_switch_iface_impl.h>
#include <remote_gate_control/generic_time_iface_impl.h>
#include <remote_gate_control/generic_io_iface_impl.h>
#include <remote_gate_control/generic_serial_iface_impl.h>

static rgc_receiver_handle_t rgc_receiver;

void setup() 
{
    generic_switch_iface_init_data_t switch_init_data =
    {
        .pin_num = 4
    };
    generic_iface_handle_t switch_handle = generic_switch_iface_ctor_dtor.create(&switch_init_data);

    generic_iface_handle_t time_iface_handle = generic_time_iface_ctor_dtor.create(NULL);

    generic_iface_handle_t serial_iface_handle = generic_serial_iface_ctor_dtor.create(NULL);

    generic_io_iface_init_data_t io_iface_init_data =
    {
        .serial_iface_handle = serial_iface_handle,
        .time_iface_handle   = time_iface_handle
    };

    generic_iface_handle_t io_iface_handle = generic_io_iface_ctor_dtor.create(&io_iface_init_data);

    rgc_receiver_init_data_t rgc_receiver_init_data =
    {
        .switch_handle     = switch_handle,
        .generic_io_handle = io_iface_handle,
        .time_iface_handle = time_iface_handle,
        .trigger_delay     = 200
    };

    rgc_receiver = rgc_receiver_init(&rgc_receiver_init_data);
}

void loop()
{
    rgc_receiver_tick(rgc_receiver);
}