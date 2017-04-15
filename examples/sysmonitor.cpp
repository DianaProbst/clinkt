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
  FILE* file;
  file = fopen("/proc/stat", "r");
  fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow, &lastTotalSys, &lastTotalIdle);
  fclose(file);
}

int getCPUValue(){
  long long percent;  // we're converting to 8 pixels so don't require much precision, but we increase our errors later, not now
  FILE* file;
  unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

  file = fopen("/proc/stat", "r");
  fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow, &totalSys, &totalIdle);
  fclose(file);

  if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow || totalSys < lastTotalSys || totalIdle < lastTotalIdle)
    { // overflow detection
      percent = -1;
    }
  else if(totalUser == lastTotalUser && totalUserLow == lastTotalUserLow && totalSys == lastTotalSys && totalIdle == lastTotalIdle)
    {return  0;}
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

int parseLine(char* line){
  // assumes a digit will be found and the line ends in ' kB'
  int i = strlen(line);
  const char* p = line;
  while (*p < '0' || *p > '9') p++;
  line[i-3] = '\0';
  i = atoi(p);
  return i;
}

int getMemValue(){
  FILE* file;
  uint32_t memTotal, memAvailable;
  file = fopen("/proc/meminfo", "r");     // frankly astounded if this works
  char line[128];
  int result = -1;
  
  while (fgets(line, 128, file) != NULL)
    { if (strncmp(line, "MemTotal:", 9) == 0){
	memTotal = parseLine(line);
	break;
      }
    }
  while (fgets(line, 128, file) != NULL)
    { if (strncmp(line, "MemAvailable:", 13) == 0){
	memAvailable = parseLine(line);
	break;
      }
    }
  fclose(file);
  result = (memTotal - memAvailable);
  return 8 * result / memTotal;
}

int main(){

  if (start()){
    std::cout << "Unable to start apa102: bcm not initialising?\n";
    return 1;
  }

  signal(SIGINT, dieNicely);   // On keyboard interrupt, darkens pixels and stops gpio using exit(0)

  init();
  
  srand(time(NULL));
  
  PixelList CPU(NUM_LEDS);            // grab from /proc/stat
  for (int i = 0; i < NUM_LEDS; i++)
    { CPU.setP(3, i);}
  PixelList NextCPU = CPU;
  
  PixelList Mem(NUM_LEDS);            // grab from /proc/meminfo
  for (int i = 0; i < NUM_LEDS; i++)
    { Mem.setP(3, i);}
  PixelList NextMem = Mem;           
 
  while (true)
    {
      int shieldStrength = getCPUValue();
      if (shieldStrength == -1)
	{ // something is pretty damned wrong
	  break;
	}
      shieldStrength /= 12;  // approximate mathematics
      shieldStrength = shieldStrength > 8 ? 8 : shieldStrength;
      shieldStrength += 1;   // A light to say the program has come on is good
      for (int i = 0; i < shieldStrength; i++)
	{
	  CPU.setFullPixel(0xFF000003, i);
	}
      for (int i = shieldStrength; i < NUM_LEDS; i++)
	{
	  CPU.setFullPixel(0x00000000, i);
	}
      Mem.crossfade(CPU,7);
      Mem.show();
      usleep(1000000);
            
      int laserStrength = getMemValue();
      if (laserStrength == -1)
	{
	  break;
	}
      laserStrength += 1;
      for (int i = 0; i < laserStrength; i++)
	{
	  Mem.setFullPixel(0x0000FF03, i);
	}
      for (int i = laserStrength; i < NUM_LEDS; i++)
	{
	  Mem.setFullPixel(0x00000000, i);
	}
      CPU.crossfade(Mem,7);
      CPU.show();
      usleep(1000000);
    }
      
  stop(); // ends gpio nicely in the highly strange event of us getting here
  
  return 0;	
}
#endif
