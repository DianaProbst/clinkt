//-------------------------------------------------------------------------------//
//                                                                               //
//       NB: Use of this graph as a monitor relies on having something           //
//           to monitor.  It is however permissable to randomise it              // 
//           and pretend to be piloting a spaceship.                             //
//                                                                               //
//           Testing's going well, thanks.                                       //
//                                                                               //
//-------------------------------------------------------------------------------//

/** Shows the use of dual pixel grids in crossfade(), and cycling of multiple patterns **/


#include <bcm2835.h>  // communicate with gpio
#include <stdint.h>   // fixed width ints to hold Pixel values
#include <unistd.h>   // usleep
#include <stdlib.h>   // basically everything I think
#include "../lib/pixel.h"    // Pixel and PixelList classes
#include "../lib/clinkt.h"   // just leave this one in
#include <iostream>   // genuinely only here to output the word 'Dead'

#include "stdio.h"
#include <sys/time.h>
#include <sys/resource.h>
#include "string.h"

#ifndef TEST

  static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

  void init(){
    FILE* file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow, &lastTotalSys, &lastTotalIdle);
    fclose(file);
  }

  int getCurrentValue(){
    long long percent;  // we're converting to 8 pixels so don't require much precision
    FILE* file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow, &totalSys, &totalIdle);
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow || totalSys < lastTotalSys || totalIdle < lastTotalIdle)
      { // overflow detection
	percent = -1;
      }
    else { total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) + (totalSys - lastTotalSys);
      percent = total * 100;
      total += (totalIdle - lastTotalIdle);
      percent /= total;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    return (int)percent;
  }


int main(){
  
  if (start()){
    std::cout << "Unable to start apa102: bcm not initialising?\n";
    return 1;
  }

  signal(SIGINT, dieNicely);   // On keyboard interrupt, darkens pixels and stops gpio using exit(0)

  srand(time(NULL));
  
  PixelList CPU(NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++)
    { CPU.setP(3, i);}

  PixelList NextCPU = CPU;
  
  PixelList Mem(NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++)
    { CPU.setP(3, i);}


  PixelList Temp(NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++)
    { CPU.setP(3, i);}
  
  while (true)
    {
      int shieldStrength = getCurrentValue();
      if (shieldStrength == -1)
	{continue;
	}
      shieldStrength /= 12;  // A little overflow never hurt anybody
      shieldStrength += 1;   // A light to say the program has come on is good
      for (int i = 0; i < shieldStrength; i++)
	{
	  CPU.setFullPixel(0xFF000003, i);
	}
      for (int i = shieldStrength; i < NUM_LEDS; i++)
	{
	  CPU.setFullPixel(0x00000000, i);
	}
      NextCPU.crossfade(CPU,10);
      NextCPU.show();
      usleep(1000000);
      CPU = NextCPU;
    }
  
  stop(); // ends gpio nicely in the highly strange event of us getting here
  
  return 0;	
}
#endif
