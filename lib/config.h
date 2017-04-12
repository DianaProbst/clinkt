// Contains const defintions of things like default pixel number, default brightness, and so on.  Turning points for rectangular pixel arrays also probably go here.


// Include Guard ------------------------------------------------------------------------
#ifndef CONFIG_H
#define CONFIG_H


// communication with a Raspberry Pi
const int MOSI=23;
const int SCLK=24;



// Constants for defaults in functions
const int NUM_LEDS = 8;
const uint8_t defaultBrightness = 3;





#endif // CONFIG_H
