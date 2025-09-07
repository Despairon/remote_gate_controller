#include <Arduino.h>
extern "C"
{
    #include <rgc_receiver.h>
}

void setup() 
{
    rgc_receiver_init();
}

void loop()
{
    rgc_receiver_tick();
}