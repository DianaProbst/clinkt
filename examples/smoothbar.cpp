//-------------------------------------------------------------------------------//
//                                                                               //
//       NB: In the interests of cross-Atlantic friendship I have                //
//           avoided embedding the word 'colour' in the spelling                 //
//           of this example.  even though it is about colour.                   //
//                                                                               //
//           As far as spelling 'maths' goes, I just give up                     //
//                                                                               //
//-------------------------------------------------------------------------------//

/** This library is intended to show a simple use of maths in designing a colour pattern
 **/


#include <bcm2835.h>  // communicate with gpio
#include <stdint.h>   // fixed width ints to hold Pixel values
#include <unistd.h>   // usleep
#include <stdlib.h>   // basically everything I think   definitely ramd
#include "../lib/pixel.h"    // Pixel and PixelList classes
#include "../lib/clinkt.h"   // just leave this one in
#include <iostream>   // genuinely only here to output the word 'Dead'
#include "../lib/low_level.h"// signal, flushBuffer, and other goodies

#include <cmath> // show me a sine!

const int NUM_LEDS = 8;
// !! Const Ints belong in a header file

#define PI 3.14159265

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
  float radians;
  uint8_t brightness = 3;
  uint8_t redness;
  uint8_t greenness;
  uint8_t blueness;
  uint8_t sector = 0;

  //cycle around hues   
  while (true)
    {
      for (int i = 0; i < 6; i++)
	{ 
	  sector %= 6;
	  sector ++;
	  for (int degrees = 0; degrees < 60; degrees++)
	    {
	      radians = (degrees * 180/60) * PI / 180;
	      uint8_t cosFun = (((int)(255 * cos(radians)) + 256)/2) % 256;
	      uint8_t high = 255;
	      uint8_t low = 0;
	  
	      switch (sector) {
	      case 1: redness = high; greenness = (255 - cosFun); blueness = low; break;
	      case 2: redness = cosFun; greenness = high; blueness = low; break;
	      case 3: redness = low; greenness = high; blueness = (255 - cosFun); break; 
	      case 4: redness = low; greenness = cosFun; blueness = high; break;
	      case 5: redness = (255-cosFun); greenness = low; blueness = high; break; 
	      case 6: redness = high; greenness = low; blueness = cosFun; break;
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
    }
  stop(); // ends gpio nicely in the highly strange event of us getting here
  
  return 0;	
}
#endif
