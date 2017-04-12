//-------------------------------------------------------------------------------//
//                                                                               //
//       NB: Raspberry Pi does not have a hardware clock and so this library     //
//           will require datetime messing on most OSs                           //
//                                                                               //
//-------------------------------------------------------------------------------//

/** This library is intended to show a use-case of Pixel(r,g,b,br) amd later recall via .getPixel()
 **/


#include <bcm2835.h>  // communicate with gpio
#include <stdint.h>   // fixed width ints to hold Pixel values
#include <unistd.h>   // usleep
#include <stdlib.h>   // basically everything I think
#include "../lib/pixel.h"    // Pixel and PixelList classes
#include "../lib/clinkt.h"   // just leave this one in
#include <iostream>   // genuinely only here to output the word 'Dead'
#include "../lib/low_level.h"// signsl, flushBuffer, and other goodies

#include <array>      // a ray
#include <time.h>     // everything to do with clock time and time storage

const int bright = 1; // We will be looking at this a lot and the pixels are blinding

#ifndef TEST

// establish a look-up array of potential pixel colours.  (Selected semi-arbitrarily.)
Pixel digits [10] = {Pixel(255, 0, 0, bright)
		     , Pixel(255, 127, 0, bright)
		     , Pixel(255, 255, 0, bright)
		     , Pixel(127, 255, 0, bright)
		     , Pixel(0, 255, 0, bright)
		     , Pixel(0, 255, 127, bright)
		     , Pixel(0, 255, 255, bright)
		     , Pixel(0, 127, 255, bright)
		     , Pixel(0, 0, 255, bright)
		     , Pixel(127, 0, 255, bright)  };


  // convert int result of tens/units calc to RGBbr value in uint32_t format
inline uint32_t convertPixel(int t)
{
  uint32_t result = digits[t].getPixel();
  return result;
}


int main(){
  
  if (start()){
    std::cout << "Unable to start apa102: bcm not initialising?\n";
    return 1;
  }
  
  // defaults and useful pixels
  
  PixelList Clock;

  signal(SIGINT, dieNicely);   // On keyboard interrupt, darkens pixels and stops gpio
                               // uses exit(0) rather than clutter code elsewhere
  
  // time management
  time_t rawtime;
  struct tm * timeinfo;  // How we manage keeping the hours, minutes, seconds in order
  uint32_t seconds;
  uint32_t secondsUnits;
  uint32_t secondsTens;
  uint32_t minutes;
  uint32_t minutesUnits;
  uint32_t minutesTens;
  uint32_t hours;
  uint32_t hoursUnits;
  uint32_t hoursTens;
  

  
  while (true)
    {
      time(&rawtime);  // get current time
      timeinfo = localtime (&rawtime);

      seconds = timeinfo->tm_sec;
      secondsUnits = seconds % 10;
      secondsTens = seconds/10;

      minutes = timeinfo->tm_min;
      minutesUnits = minutes % 10;
      minutesTens = minutes / 10;

      hours = timeinfo->tm_hour;
      hoursUnits = hours % 10;
      hoursTens = hours / 10;


      setPixel(Clock, convertPixel(secondsUnits), 0);       // using our function
      setPixel(Clock, digits[secondsTens].getPixel(), 1);   // or a direct call to Pixel 
      setPixel(Clock, 0, 2);                                // blank space
      
      setPixel(Clock, convertPixel(minutesUnits), 3);
      setPixel(Clock, convertPixel(minutesTens), 4);
      setPixel(Clock, 0, 5);
     
      setPixel(Clock, convertPixel(hoursUnits), 6);
      
      if (hours < 10)                                       // only one digit if it's still single-digit time
	{ setPixel(Clock, 0, 7);
	}
      else
	{ setPixel(Clock, digits[hoursTens].getPixel(), 7);
	}
      Clock.show();
      usleep(1000000);
    }
  
  stop(); // ends gpio nicely in the highly strange event of us getting here
  
  return 0;	
}
#endif
