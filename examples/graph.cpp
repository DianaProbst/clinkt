//-------------------------------------------------------------------------------//
//                                                                               //
//       NB: In the interests of cross-Atlantic friendship I have                //
//           avoided embedding the word 'colour' in the spelling                 //
//           of this example.  even though it is about colour.                   //
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

  signal(SIGINT, dieNicely);   // On keyboard interrupt, darkens pixels and stops gpio
                               // uses exit(0) - write your own if that is a deal-breaker

  PixelList Graph(NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++)
    { Graph.setP(3, i);}


  // replace NUM_LEDS with whatever input you desire

  while (true)
    {
      // poll something interesting or check an interrupt
      
      for (int i = 0; i < NUM_LEDS; i++)
	
	{ Graph.setP(0x000000001, i);
	  Graph.show();
	  usleep(100000);
	}

      for (int i = 0; i < NUM_LEDS; i++)
	
	{ Graph.setP(0x00000ff01, NUM_LEDS - (i+1));
	  Graph.show();
	  usleep(100000);
	}
      usleep(100000);
    }

  stop(); // ends gpio nicely in the highly strange event of us getting here
  
  return 0;	
}
#endif
