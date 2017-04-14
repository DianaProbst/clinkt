This library depends on a PixelList class and a Pixel class with several exposed functions.  Arbitrary pixel lists can be used and kept, allowing for patterns to be built.  The library can drive any number of pixels that can be powered, in any rectangular configuration, with a little tweaking of the config.h file.

Requirements;

requires bcm2835 library to communicate with pi gpio.

program control functions

sigint_handler(n)  // used in testing, when communicating with bcm chip

start()  // required at the start of main() to initiate and check gpio

stop()   // clears all data and prepares for close gracefully

non-exposed:
	write_byte(n) // instruction to bcm chip to output to gpio


PixelList Functions

PixelList MyList  // creates new pixel list with NUM_LED pixels

PixelList MyList(n)  // creates a new pixel list with n pixels

PixelList.fade(m)  // fades until gone with each step-decrease taking m millisecs

PixelList.rise(m, b)  // pattern rises to brightness b im m-millisec steps

A default PixelList 'Blinkt' is created at compile time in int main()
Unless another PixelList is created and passed where appropriate, all operations are on Blinkt.


pixel functions:

selectPixel(int p) // p as int pixel position

setColour(uint8_t r, uint8_t g, uint8_t b) // sets colour of selected pixel with no change in brightness value

copyPixel(int P) // copies pixel P to selected pixel

setBrightness(uint8_t br) // brightness as 0-7

clearPixel(p) // sets pixel p to 0, 0, 0 with default brightness

clearAll()  // runs clearPixel on all pixels

lightsOff() // sets all pixels in LED strip off

Convenience Function:
setPixel(PixelList PL, int p, uint8_t r, uint8_t g, uint8_t b, uint8_t br)  // p as int pixel position, rgb values as 0-255, brightness as 0-7





p.getPixel()  // returns colour/brightness setting of p# as a uint32_t

show()  // outputs all stored pixel values to blinkt via array of all classes


underlying functions:  // these are not directly exposed

p#.uint32_rgbbr(r, g, b, br) // calculates and returns uint32_t representing full pixel r-g-b-br uint8_t inputs

p#.uint32_rgb(r, g, b) // calculates and returns uint32_rgbbr, with no change in brightness

