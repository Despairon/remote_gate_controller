#include <Arduino.h>
extern "C"
{
    #include <rgc_transmitter.h>
}

void setup() 
{
    rgc_transmitter_init();
}

void loop()
{
    rgc_transmitter_tick();
}