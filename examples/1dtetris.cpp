//-------------------------------------------------------------------------------//
//                                                                               //
//       NB: I didn't think of this.                                             //
//                                                                               //
//           But I really really really wish I had.                              //
//                                                                               //
//-------------------------------------------------------------------------------//

/** This library is intended to show a use-case of a PixelList holding a shape
  It also demos fade() and rise()

  And apparently a malloc failure on exit, but what's that between friends?
 **/


#include <bcm2835.h>  // communicate with gpio
#include <stdint.h>   // fixed width ints to hold Pixel values
#include <unistd.h>   // usleep
#include <stdlib.h>
#include "../lib/pixel.h"    // Pixel and PixelList classes
#include "../lib/clinkt.h"   // just leave this one in
#include <iostream>   // genuinely only here to output the word 'Dead'
#include "../lib/low_level.h"// signal, flushBuffer, and other goodies

#ifndef TEST

int main(){
  
  if (start()){
    std::cout << "Unable to start apa102: bcm not initialising?\n";
    return 1;
  }

  signal(SIGINT, dieNicely);   // On keyboard interrupt, darkens pixels and stops gpio
                               // uses exit(0) - write your own if that is a deal-breaker
  
  // randomish seed
  srand (time(NULL));

  // defaults and useful pixels
  PixelList Grid(NUM_LEDS + 5);
  int length;

  
  
  while (true)
    { 

      for (int i = 0; i < (NUM_LEDS+5); i++)
	{
	  setPixel(Grid, 0, i);
	}

      // generate length and fill pixels 8+ including dark pixel behind

      length = rand() % 4 + 1;  // RAND_MAX is a const defined in cstdlib
      
      uint32_t colour = (rand() % 0b111111111111111111111111) << 8;
      colour += 0b11; //!! arbitrary number
      for (int i = NUM_LEDS; i < NUM_LEDS + length; i++)
	{ setPixel(Grid, colour, i) ;}

      // copy shape downwards until at 0
      while (true)
	{ for (int i = 0; i < NUM_LEDS + length; i++)
	    {
	      setPixel(Grid, Grid.getPixel(i+1), i);
	    }
	  Grid.show();
	  usleep(500000);
	  if (Grid.getPixel(0) != 0) { break; }
	}

      Grid.fade(100);  // millisecs between each step in the fade
      Grid.rise(100, 3); // millisecs between each step
      Grid.fade(100);   // fade will fade out until there is nothing left, step by step
      usleep(250000);
      
    }
  
  stop(); // ends gpio nicely in the highly strange event of us getting here
  
  return 0;	
}
#endif
