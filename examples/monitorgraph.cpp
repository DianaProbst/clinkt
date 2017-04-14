//-------------------------------------------------------------------------------//
//                                                                               //
//       NB: Use of this graph as a monitor relies on having something           //
//           to monitor.  It is however permissable to randomise it              // 
//           and pretend to be piloting a spaceship.                             //
//                                                                               //
//           Testing's going well, thanks.                                       //
//                                                                               //
//-------------------------------------------------------------------------------//

/** Shows the use of a pixel grid as a graph
 **/


#include <bcm2835.h>  // communicate with gpio
#include <stdint.h>   // fixed width ints to hold Pixel values
#include <unistd.h>   // usleep
#include <stdlib.h>   // basically everything I think
#include "../lib/pixel.h"    // Pixel and PixelList classes
#include "../lib/clinkt.h"   // just leave this one in
#include <iostream>   // genuinely only here to output the word 'Dead'


#ifndef TEST

int main(){
  
  if (start()){
    std::cout << "Unable to start apa102: bcm not initialising?\n";
    return 1;
  }

  signal(SIGINT, dieNicely);   // On keyboard interrupt, darkens pixels and stops gpio using exit(0)

  srand(time(NULL));
  
  PixelList Shields(NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++)
    { Shields.setP(3, i);}

  PixelList ActiveShields(NUM_LEDS);
  
  int shieldStrength;
  
  // replace NUM_LEDS with whatever input you desire

  while (true)
    {
      shieldStrength = (rand() % 8) + 1;
      for (int i = 0; i < shieldStrength; i++)
	{
	  ActiveShields.setFullPixel(0x0000FF03, i);
	}
      for (int i = shieldStrength; i < NUM_LEDS; i++)
	{
	  ActiveShields.setFullPixel(0x00000000, i);
	}
      Shields.crossfade(ActiveShields);
      Shields.show();
      usleep(1000000);
      Shields = ActiveShields;
    }
  
  stop(); // ends gpio nicely in the highly strange event of us getting here
  
  return 0;	
}
#endif
