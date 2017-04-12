//-------------------------------------------------------------------------------//
//                                                                               //
//       NB: In the interests of cross-Atlantic friendship I have                //
//           avoided embedding the word 'colour' in the spelling                 //
//           of this example.  even though it is about colour.                   //
//                                                                               //
//-------------------------------------------------------------------------------//

/** Shows a bar of pixels which cycle together around a colour wheel
 **/


#include <bcm2835.h>  // communicate with gpio
#include <stdint.h>   // fixed width ints to hold Pixel values
#include <unistd.h>   // usleep
#include <stdlib.h>   // basically everything I think   definitely ramd
#include "../lib/pixel.h"    // Pixel and PixelList classes
#include "../lib/clinkt.h"   // just leave this one in
#include <iostream>   // genuinely only here to output the word 'Dead'
#include "../lib/low_level.h"// signal, flushBuffer,


const int NUM_LEDS = 8;
// !! Const Ints belong in a header file

#ifndef TEST

int main(){
  
  if (start()){
    std::cout << "Unable to start apa102: bcm not initialising?\n";
    return 1;
  }

  signal(SIGINT, dieNicely);   // On keyboard interrupt, darkens pixels and stops gpio
                               // uses exit(0) - write your own if that is a deal-breaker

  PixelList Wheel(NUM_LEDS);
  Pixel temp;
  uint8_t sector;
  uint8_t brightness = 3;
  uint8_t redness;
  uint8_t greenness;
  uint8_t blueness;

  //cycle through colour space wheeeeeeeeeee
  while (true)
    {
      for (int degrees = 1; degrees <= 360; degrees++)  // the hue wheel is in 360 parts
	{
	  // hue wheel can be divided into 6 parts, unlike Gaul
	  sector = (degrees/60) + 1;

	  uint8_t rising = degrees % 60;
	  uint8_t falling = 60-(degrees % 60);
	  uint8_t high = 60;
	  uint8_t low = 0;
	  
	  // this has saturation correct but lightness between 61 and 120 - scaling is your friend
	  
	  switch (sector) {
	  case 1: redness = high; greenness = rising; blueness = low; break; // red high, green rising
	  case 2: redness = falling; greenness = high; blueness = low; break; // green high, red falling
	  case 3: redness = low; greenness = high; blueness = rising; break; // green high, blue rising
	  case 4: redness = low; greenness = falling; blueness = high; break; // blue high, green falling
	  case 5: redness = rising; greenness = low; blueness = high; break; // blue high, red rising
	  case 6: redness = high; greenness = low; blueness = falling; break; // red high, blue falling
	  }
	  
	  temp.setP(redness, greenness, blueness, brightness);
	  
	  for (int j = 0; j < NUM_LEDS; j++)
	    {    
	      setPixel(Wheel, temp.getPixel(), j);
	    }

	 Wheel.show();
	 usleep(10000);
	}
    }

  stop(); // ends gpio nicely in the highly strange event of us getting here
  
  return 0;	
}
#endif
