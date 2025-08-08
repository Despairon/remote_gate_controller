#include <Arduino.h>
extern "C"
{    
    #include <remote_gate_control/rgc_receiver.h>
}

void setup() 
{
    rgc_receiver_init();
}

void loop()
{
    rgc_receiver_tick();
}