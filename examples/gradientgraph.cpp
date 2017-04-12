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
#include <stdlib.h>   // basically everything I think   definitely rand
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
                               // uses exit(0) - write your own if OS clean-up isn't good enough for you

  PixelList Graph(NUM_LEDS);  // set by hand to make it pretty in whatever gradiet you wish
  Graph.setP(0xFF000001, 0);
  Graph.setP(0xFF770001, 1);
  Graph.setP(0x77FF0001, 2);
  Graph.setP(0x00FF0001, 3);
  Graph.setP(0x00FF7701, 4);
  Graph.setP(0x00FFFF01, 5);
  Graph.setP(0x0077FF01, 6);
  Graph.setP(0x0000FF01, 7);
  
  PixelList Mask;

  int target;
  srand(time(NULL)); 
  
  while (true)
    {
      for (int i = 0; i < NUM_LEDS; i++)
	{Mask.setP(0, i);}
      
      target = rand() % 8 + 1;
      // poll something interesting or check an interrupt
      
      for (int i = 0; i < target; i++)
	
	{
	  Mask.setP(Graph.getPixel(i), i);
	  Mask.show();
	  usleep(100000);
	}

      for (int i = 0; i < target; i++)

	{ Mask.setP(0, target - (i+1));
	  Mask.show();
	  usleep(100000);
	}
    }

  stop(); // ends gpio nicely in the highly strange event of us getting here
  
  return 0;	
}
#endif
