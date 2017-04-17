//-------------------------------------------------------------------------------//
//                                                                               //
//       NB: Cheerlights are a friendly way of sharing a single colour           //
//                                                                               //
//           I am not friendly, but feed me enough tea and I will pretend        //
//                                                                               //
//-------------------------------------------------------------------------------//

/** Shows a bar of pixels coloured based on the cheerlights channel on thingspeak.com

thingspeak has public channels acceible without an API key

 **/


#include <bcm2835.h>  // communicate with gpio
#include <stdint.h>   // fixed width ints to hold Pixel values
#include <unistd.h>   // usleep
#include <stdlib.h>   // basically everything I think   definitely ramd
#include "../lib/pixel.h"    // Pixel and PixelList classes
#include "../lib/clinkt.h"   // just leave this one in
#include <iostream>   // genuinely only here to output the word 'Dead'
#include "../lib/low_level.h"// signal, flushBuffer,

#include <curl/curl.h>  // sudo apt-get install libcurl4-openssl-dev or your local equivalent
                        // add -l curl to Makefile

#ifndef TEST

// takes contents of curl object and converts to a string
static size_t writeCallBack(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string*)userp)->clear();
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

int main(){
  
  if (start()){
    std::cout << "Unable to start apa102: bcm not initialising?\n";
    return 1;
  }
  
  curl_global_init( CURL_GLOBAL_ALL );

  CURL* myHandle;
  CURLcode result;   // error code stored here
  std::string readBuffer;
  myHandle = curl_easy_init(); // curl 'easy' mode
  curl_easy_setopt(myHandle, CURLOPT_URL, "https://api.thingspeak.com/channels/1417/field/2/last.txt");
  curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, writeCallBack);
  curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, &readBuffer);
  
  signal(SIGINT, dieNicely);   // On keyboard interrupt, darkens pixels and stops gpio
                               // uses exit(0) - write your own if that is a deal-breaker

  PixelList Bar(NUM_LEDS);
  Pixel temp;
  uint8_t brightness = 3;

  //cycle through colour space wheeeeeeeeeee
  while (true)
    {
      result = curl_easy_perform(myHandle); // do the grabbing
      // grab cheerlights colour with libcurl
      if (result != 0)
	{//usleep(5000000);
	  continue;}  // try again

      temp.setHexPixel(readBuffer, brightness);
      
      for (int i = 0; i < NUM_LEDS; i++)
	{    
	  setPixel(Bar, temp.getPixel(), i);
	}

      Bar.show();
      usleep(10000);
    }
    
  curl_easy_cleanup(myHandle);   // if we get here something is badly wrong with the entire universe
  stop(); // ends gpio nicely in the highly strange event of us getting here
  
  return 0;	
}
#endif
