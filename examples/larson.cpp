//-------------------------------------------------------------------------------//
//                                                                               //
//       NB: Only people who had a television in the 1980s are                   //
//           expected to find this cool.                                         //
//                                                                               //
//           (This one's for you, Brian)                                         //
//                                                                               //
//-------------------------------------------------------------------------------//

/** This library is intended to show a use-case of a moving shape that vanishes
      off the ends of the LED strip
 **/


#include <bcm2835.h>  // communicate with gpio
#include <stdint.h>   // fixed width ints to hold Pixel values
#include <unistd.h>   // usleep
#include <stdlib.h>   // basically everything I think   definitely ramd
#include "../lib/pixel.h"    // Pixel and PixelList classes
#include "../lib/clinkt.h"   // just leave this one in
#include <iostream>   // genuinely only here to output the word 'Dead'
#include "../lib/low_level.h"// signal, flushBuffer, and other goodies

#include <array>  // size() I think


#ifndef TEST

int main(){
  
  if (start()){
    std::cout << "Unable to start apa102: bcm not initialising?\n";
    return 1;
  }

  signal(SIGINT, dieNicely);   // On keyboard interrupt, darkens pixels and stops gpio
                               // uses exit(0) - write your own if that is a deal-breaker

  std::array<int, 15>pattern {0, 0, 0, 0, 0, 0, 0, 0, 4, 16, 64, 255, 64, 16, 4};

  PixelList Scanner(NUM_LEDS);
  Pixel temp;
  
  while (true)
    {
      for (uint8_t i = 0; i < pattern.size(); i++)
	{
	  for (int j = 0; j < NUM_LEDS; j++)
	    {
	      temp.setP(pattern[(j + i) % pattern.size()], 0, 0, 6); 
	      setPixel(Scanner, temp.getPixel(), j); // for off-screen buffer see 1dtetris.cpp
	    }
	  Scanner.show();
	  usleep(250000);
	}
    }
  stop(); // ends gpio nicely in the highly strange event of us getting here
  
  return 0;	
}
#endif
