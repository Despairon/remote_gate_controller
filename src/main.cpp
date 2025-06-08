#include <Arduino.h>
extern "C"
{    
    #include <remote_gate_control/rgc_receiver.h>
}
#include <remote_gate_control/gpio_iface_impl.h>
#include <remote_gate_control/time_iface_impl.h>
#include <remote_gate_control/receiver_io_iface_impl.h>
#include <remote_gate_control/uart_iface_impl.h>

static rgc_receiver_handle_t rgc_receiver;

void setup() 
{
    gpio_iface_init_data_t gpio_init_data =
    {
        .pin_num = 4
    };
    iface_ctx_t gpio_hdl = gpio_iface.init.create(&gpio_init_data);

    iface_ctx_t time_iface_hdl = time_iface_init.create(NULL);

    iface_ctx_t uart_hdl = uart_iface_init.create(NULL);

    generic_io_init_data_t io_iface_init_data =
    {
        .uart_hdl       = uart_hdl,
        .time_iface_hdl = time_iface_hdl
    };

    iface_ctx_t io_iface_hdl = generic_io_init.create(&io_iface_init_data);

    rgc_receiver_init_data_t rgc_receiver_init_data =
    {
        .gpio_hdl        = gpio_hdl,
        .receiver_io_hdl = io_iface_hdl,
        .time_iface_hdl  = time_iface_hdl,
        .trigger_delay   = 200
    };

    rgc_receiver = rgc_receiver_init(&rgc_receiver_init_data);
}

void loop()
{
    rgc_receiver_tick(rgc_receiver);
}